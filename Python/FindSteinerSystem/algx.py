#from https://www.cs.mcgill.ca/~aassaf9/python/algorithm_x.html

def solve(X, Y, solution=[]):
  """ 
  X is a set or list. 
  Y is a dictionary whose values are lists of elements of X. 
  Returns a list of keys of Y whose values are a partition of X.
  """
  if type(X) is not dict:
    X = {j: set() for j in X}
    for i in Y:
      for j in Y[i]:
        X[j].add(i)
        
  if not X:
    yield list(solution)
  else:
    c = min(X, key=lambda c: len(X[c]))
    for r in list(X[c]):
      solution.append(r)
      cols = select(X, Y, r)
      for s in solve(X, Y, solution):
        yield s
      deselect(X, Y, r, cols)
      solution.pop()

def select(X, Y, r):
  cols = []
  for j in Y[r]:
    for i in X[j]:
      for k in Y[i]:
        if k != j:
          X[k].remove(i)
    cols.append(X.pop(j))
  return cols

def deselect(X, Y, r, cols):
  for j in reversed(Y[r]):
    X[j] = cols.pop()
    for i in X[j]:
      for k in Y[i]:
        if k != j:
          X[k].add(i)
