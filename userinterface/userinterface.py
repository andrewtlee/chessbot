# Andrew Lee
# 3 May 2021
# userinterface.py
"""
This python module interfaces with the C++ Chessbot Engine to play chess.
Or at least, it will eventually...
"""
import chessbot
import tkinter as tk
import tkinter.messagebox
import time

sizeOfBoard = 720
symbolSize = (sizeOfBoard / 8)
symbolThickness = 50
piecesAsUnicode = {0: "", 1: "\u2659", 2: "\u2658", 3:"\u2657", 5: "\u2656", 8: "\u2655", 9: "\u2654",
                   -1: "\u265F", -2: "\u265E", -3: "\u265D", -5: "\u265C", -8: "\u265B", -9: "\u265A"}
whiteToMove = 0x01
OFFBOARD = -128

times = []

class Chess:
    def __init__(self):
        self.window = tk.Tk()
        self.window.title("Chessbot (Alpha)")
        self.canvas = tk.Canvas(self.window, width=sizeOfBoard, height=sizeOfBoard)
        self.canvas.pack()
        self.window.bind("<Button-1>", self.onClick)
        self.window.protocol("WM_DELETE_WINDOW", self.onClose)
        self.board = chessbot.getCurrentBoard()
        self.gameCtrlFlags = chessbot.getGameCtrlFlags()
        self.pieceHandles = []
        self.initializeBoard()
        self.window.update()
        
        self.resetBoard = False
        self.whiteWins = False
        self.blackWins = False
        self.AIisWhite = False
        self.AIisBlack = True
        self.waitingOnPlayer = False
        self.windowClosed = False
        self.logicalClickStack = [[-1, -1], [-1,-1]]
        self.prospectiveMoveBoard = self.board[:]
        self.prospectiveMovePiece = OFFBOARD

    def mainloop(self):
        while not self.windowClosed:
            if self.gameCtrlFlags & whiteToMove:
                if self.AIisWhite:
                    start = time.time()
                    chessbot.makeAutomaticMove(5)
                    end = time.time()
                    self.prospectiveMoveBoard = chessbot.getCurrentBoard()
                    print(f"{end-start} spent generating move tree") 
                    times.append(end-start)
                else:
                    self.waitingOnPlayer = True
                    if not self.waitingOnPlayer:
                        self.logicalClickStack = [[-1, -1], [-1,-1]]
                        self.prospectiveMoveBoard = chessbot.getCurrentBoard()
                        self.prospectiveMovePiece = OFFBOARD                   
            else:
                if self.AIisBlack:
                    start = time.time()
                    chessbot.makeAutomaticMove(5)
                    end = time.time()
                    self.prospectiveMoveBoard = chessbot.getCurrentBoard()
                    times.append(end-start)
                else:
                    self.waitingOnPlayer = True
                    if not self.waitingOnPlayer:
                        self.logicalClickStack = [[-1, -1], [-1,-1]]
                        self.prospectiveMoveBoard = chessbot.getCurrentBoard()
                        self.prospectiveMovePiece = OFFBOARD
            self.board = chessbot.getCurrentBoard()
            self.gameCtrlFlags = chessbot.getGameCtrlFlags()
            self.drawBoard()
            if chessbot.blackCheckmated():
                self.window.bell()
                tkinter.messagebox.showinfo("Game Over", "White Wins!")
                exit(0)
            elif chessbot.whiteCheckmated():
                self.window.bell()
                tkinter.messagebox.showinfo("Game Over", "Black Wins!")
                exit(0)
            elif chessbot.stalemate() or chessbot.thriceRepetition():
                self.window.bell()
                tkinter.messagebox.showinfo("Game Over", "The game is a draw!")
                print(sum(times)/len(times))
                exit(0)
            self.window.update_idletasks()
            self.window.update()
            time.sleep(0.25)
    
    def initializeBoard(self):
        color = "white"
        for y in range(8):
            for x in range(8):
                x1 = x*symbolSize
                y1 = y*symbolSize
                x2 = x1+symbolSize
                y2 = y1+symbolSize
                self.canvas.create_rectangle((x1, y1, x2, y2), fill=color)
                if color == "white":
                    color = "green"
                else:
                    color = "white"
            if color == "white":
                color = "green"
            else:
                color = "white"

        for row in range(8):
            for col in range(8):
                c = self.getSpace(row, col)
                self.pieceHandles.append(self.canvas.create_text(col*symbolSize + symbolSize/2, (7-row)*symbolSize + symbolSize/2, font="* 60", text=piecesAsUnicode[c]))

    def drawBoard(self):
        for p in self.pieceHandles:
            self.canvas.delete(p)
        self.pieceHandles = []

        for row in range(8):
            for col in range(8):
                c = self.getSpace(row, col)
                self.pieceHandles.append(self.canvas.create_text(col*symbolSize + symbolSize/2, (7-row)*symbolSize + symbolSize/2, font="* 60", text=piecesAsUnicode[c]))
                
    def onClick(self, event):
        if self.waitingOnPlayer:
            gridPosition = [event.x, event.y]
            logicalPosition = [int(gridPosition[0] / (sizeOfBoard / 8)), int(gridPosition[1] / (sizeOfBoard /8))]
            self.logicalClickStack[1] = self.logicalClickStack[0]
            self.logicalClickStack[0] = [7-logicalPosition[1], logicalPosition[0]]
            spaceval = self.getSpace(self.logicalClickStack[1][0], self.logicalClickStack[1][1])
            self.prospectiveMovePiece = spaceval
            if self.prospectiveMovePiece != -128:
                self.prospectiveMoveBoard[logicalPosition[0] + 8 * (7-logicalPosition[1])] = self.prospectiveMovePiece
                self.prospectiveMoveBoard[self.logicalClickStack[1][1] + 8*self.logicalClickStack[1][0]] = 0
                if abs(self.prospectiveMovePiece) == 9:
                    if self.logicalClickStack[0][1] - self.logicalClickStack[1][1] == 2: # castle kingside
                        self.prospectiveMoveBoard[self.logicalClickStack[0][1] - 1  + 8*self.logicalClickStack[1][0]] = 5 if self.gameCtrlFlags & whiteToMove else -5
                        self.prospectiveMoveBoard[7 + 8*self.logicalClickStack[1][0]] = 0
                    elif self.logicalClickStack[0][1] - self.logicalClickStack[1][1] == -2: # castle queenside
                        self.prospectiveMoveBoard[self.logicalClickStack[0][1] + 1  + 8*self.logicalClickStack[0][0]] = 5 if self.gameCtrlFlags & whiteToMove else -5
                        self.prospectiveMoveBoard[0 + 8*self.logicalClickStack[1][0]] = 0
                if self.prospectiveMovePiece == 1 and self.logicalClickStack[0][0] == 7: # pawn promotion (queen only)
                    self.prospectiveMoveBoard[logicalPosition[0] + 8 * (7-logicalPosition[1])] = 8
                elif self.prospectiveMovePiece == -1 and self.logicalClickStack[0][0] == 0:
                    self.prospectiveMoveBoard[logicalPosition[0] + 8 * (7-logicalPosition[1])] = -8
                self.waitingOnPlayer = not chessbot.makeManualMove(self.prospectiveMoveBoard)
                self.prospectiveMoveBoard = chessbot.getCurrentBoard()

    def onClose(self):
        self.window.destroy()
        self.windowClosed = True

    def getSpace(self, row: int, col: int)->int:
        if( row >= 0 and row < 8 and col >= 0 and col < 8 ):
            return self.board[ col + 8 * row ]
        else:
            return OFFBOARD

def getSpace(row: int, col: int, board: list)->int:
        if( row >= 0 and row < 8 and col >= 0 and col < 8 ):
            return board[ col + 8 * row ]
        else:
            return OFFBOARD

def printBoardToTerminal(board: list):
    for i in range(7, -1, -1):
        for j in range(8):
            c = getSpace(i,j, board)
            print(f"{c:3}", end="")
        print()
    
   
if __name__ == "__main__":
    game = Chess()
    game.mainloop()