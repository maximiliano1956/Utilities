# Compila Sudoku.java creando varie .class bytecode
javac Sudoku.java

# Se si vuole lanciare il bytecode:
# java Sudoku (Sudoku.class e' la main class)

# Crea l'archivio Sudoku.jar specificando
# con l'opzione e il nome della main class (Sudoku)
jar cfe Sudoku.jar Sudoku *.class

# Lancia il .jar fornendo in input il file grid4.txt
java -jar Sudoku.jar grid4.txt
