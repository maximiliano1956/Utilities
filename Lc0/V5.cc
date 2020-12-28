
V5TrainingData Node::GetV5TrainingData(
    GameResult game_result, const PositionHistory& history,
    FillEmptyHistory fill_empty_history,
    pblczero::NetworkFormat::InputFormat input_format, float best_q,
    float best_d, float best_m) const {
  V5TrainingData result;

  // Set version.
  result.version = 5;
  result.input_format = input_format;

  // Populate planes.
  int transform;
  InputPlanes planes = EncodePositionForNN(input_format, history, 8,
                                           fill_empty_history, &transform);
  int plane_idx = 0;
  for (auto& plane : result.planes) {
    plane = ReverseBitsInBytes(planes[plane_idx++].mask);
  }

  // Populate probabilities.
  auto total_n = GetChildrenVisits();
  // Prevent garbage/invalid training data from being uploaded to server.
  // It's possible to have N=0 when there is only one legal move in position
  // (due to smart pruning).
  if (total_n == 0 && GetNumEdges() != 1) {
    throw Exception("Search generated invalid data!");
  }
  // Set illegal moves to have -1 probability.
  std::fill(std::begin(result.probabilities), std::end(result.probabilities),
            -1);
  // Set moves probabilities according to their relative amount of visits.
  for (const auto& child : Edges()) {
    result.probabilities[child.edge()->GetMove().as_nn_index(transform)] =
        total_n > 0 ? child.GetN() / static_cast<float>(total_n) : 1;
  }

  const auto& position = history.Last();
  const auto& castlings = position.GetBoard().castlings();
  // Populate castlings.
  // For non-frc trained nets, just send 1 like we used to.
  uint8_t queen_side = 1;
  uint8_t king_side = 1;
  // If frc trained, send the bit mask representing rook position.
  if (input_format == pblczero::NetworkFormat::INPUT_112_WITH_CASTLING_PLANE ||
      input_format ==
          pblczero::NetworkFormat::INPUT_112_WITH_CANONICALIZATION) {
    queen_side <<= castlings.queenside_rook();
    king_side <<= castlings.kingside_rook();
  }

  result.castling_us_ooo = castlings.we_can_000() ? queen_side : 0;
  result.castling_us_oo = castlings.we_can_00() ? king_side : 0;
  result.castling_them_ooo = castlings.they_can_000() ? queen_side : 0;
  result.castling_them_oo = castlings.they_can_00() ? king_side : 0;

  // Other params.
  if (input_format ==
      pblczero::NetworkFormat::INPUT_112_WITH_CANONICALIZATION) {
    result.side_to_move_or_enpassant =
        position.GetBoard().en_passant().as_int() >> 56;
    if ((transform & FlipTransform) != 0) {
      result.side_to_move_or_enpassant =
          ReverseBitsInBytes(result.side_to_move_or_enpassant);
    }
    // Send transform in deprecated move count so rescorer can reverse it to
    // calculate the actual move list from the input data.
    result.invariance_info =
        transform | (position.IsBlackToMove() ? (1u << 7) : 0u);
  } else {
    result.side_to_move_or_enpassant = position.IsBlackToMove() ? 1 : 0;
    result.invariance_info = 0;
  }
  result.rule50_count = position.GetRule50Ply();

  // Game result.
  if (game_result == GameResult::WHITE_WON) {
    result.result = position.IsBlackToMove() ? -1 : 1;
  } else if (game_result == GameResult::BLACK_WON) {
    result.result = position.IsBlackToMove() ? 1 : -1;
  } else {
    result.result = 0;
  }

  // Aggregate evaluation WL.
  result.root_q = -GetWL();
  result.best_q = best_q;

  // Draw probability of WDL head.
  result.root_d = GetD();
  result.best_d = best_d;

  result.root_m = GetM();
  result.best_m = best_m;

  // Unknown here - will be filled in once the full data has been collected.
  result.plies_left = 0;

  return result;
}
