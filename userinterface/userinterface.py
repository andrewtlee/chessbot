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
import random

sizeOfBoard = 720
symbolSize = (sizeOfBoard / 8)
symbolThickness = 50
piecesAsUnicode = {0: "", 1: "\u2659", 2: "\u2658", 3:"\u2657", 5: "\u2656", 8: "\u2655", 9: "\u2654",
                   -1: "\u265F", -2: "\u265E", -3: "\u265D", -5: "\u265C", -8: "\u265B", -9: "\u265A"}
columnsAsLetters = { 0: "a", 1: "b", 2: "c", 3: "d", 4: "e", 5: "f", 6: "g", 7: "h", -1: ""}
whiteToMove = 0x01
OFFBOARD = -128

class Node:
    def __init__(self, data = "") -> None:
        self.next = []
        self.move = data

    def addChild(self, data):
        found = -1
        for i, n in enumerate(self.next):
            if n.move == data:
                found = i
        if found == -1:
            self.next.append(Node(data))
        return self.next[found]
    
    def __str__(self, level=0):
        ret = str(level)+"."+level*" "+self.move + "\n"
        for child in self.next:
            ret += child.__str__(level+1)
        return ret

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
        self.inbook = True
        self.whiteWins = False
        self.blackWins = False
        self.AIisWhite = False
        self.AIisBlack = True
        self.waitingOnPlayer = False
        self.windowClosed = False
        self.logicalClickStack = [[-1, -1], [-1,-1]]
        self.moveHistory = []
        self.prospectiveMoveBoard = self.board[:]
        self.prospectiveMovePiece = OFFBOARD
        self.root = self.makeOpeningDatabase()
        self.currentNode = self.root

    def mainloop(self):
        while not self.windowClosed:
            if self.gameCtrlFlags & whiteToMove:
                if self.AIisWhite:
                    if self.inbook:
                        self.playOpening()
                        print("White opening")
                    else:
                        start = time.time()
                        self.moveHistory.append(chessbot.makeAutomaticMove(5))
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
                    if self.inbook:
                        self.playOpening()
                        print("Black opening")
                    else:
                        start = time.time()
                        self.moveHistory.append(chessbot.makeAutomaticMove(5))
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

        for col in range(8):
            for row in range(8):
                c = self.getSpace(col, row)
                self.pieceHandles.append(self.canvas.create_text(col*symbolSize + symbolSize/2, (7-row)*symbolSize + symbolSize/2, font="* 60", text=piecesAsUnicode[c]))

    def drawBoard(self):
        for p in self.pieceHandles:
            self.canvas.delete(p)
        self.pieceHandles = []

        for col in range(8):
            for row in range(8):
                c = self.getSpace(col, row)
                self.pieceHandles.append(self.canvas.create_text(col*symbolSize + symbolSize/2, (7-row)*symbolSize + symbolSize/2, font="* 60", text=piecesAsUnicode[c]))
                
    def onClick(self, event):
        if self.waitingOnPlayer:
            gridPosition = [event.x, event.y]
            logicalPosition = [int(gridPosition[0] / (sizeOfBoard / 8)), int(gridPosition[1] / (sizeOfBoard /8))]
            self.logicalClickStack[1] = self.logicalClickStack[0]
            self.logicalClickStack[0] = [logicalPosition[0], 7-logicalPosition[1]]
            move = self.convertLogicalPositionsToUCI(self.logicalClickStack[0], self.logicalClickStack[1])
            self.waitingOnPlayer = not chessbot.makeManualMove(move)
            if self.waitingOnPlayer:
                move += "q"
                chessbot.makeManualMove(move)
            if not self.waitingOnPlayer:
                self.moveHistory.append(move)
             
    
    def convertLogicalPositionsToUCI(self, clickLast: list, clickFirst: list)->str:
        return "".join([columnsAsLetters[clickFirst[0]], str(clickFirst[1]+1), columnsAsLetters[clickLast[0]], str(clickLast[1]+1)])
        

    def onClose(self):
        self.window.destroy()
        self.windowClosed = True

    def getSpace(self, col: int, row: int)->int:
        if( row >= 0 and row < 8 and col >= 0 and col < 8 ):
            return self.board[ col + 8 * row ]
        else:
            return OFFBOARD
    def makeOpeningDatabase(self) -> Node:
        root = Node()
        with open("C:/repos/chessbot/games.txt", "r") as infile:
            for game in infile.readlines():
                moves = game.split()
                child = root
                for i, move in enumerate(moves):
                    child = child.addChild(move)
                    if i == 25:
                        break
        return root

    def playOpening(self) -> bool:
        if not len(self.moveHistory):
            self.currentNode = random.choice(self.root.next)
            chessbot.makeManualMove(self.currentNode.move)
            self.moveHistory.append(self.currentNode.move)
            return True
        self.currentNode = self.root
        for move in self.moveHistory:
            self.inbook = False
            for i, booknode in enumerate(self.currentNode.next):
                if move == booknode.move:
                    self.currentNode = self.currentNode.next[i]
                    self.inbook = True
                    break
        if self.inbook:
            if len(self.currentNode.next):
                self.currentNode = random.choice(self.currentNode.next)
                if chessbot.makeManualMove(self.currentNode.move):
                    self.moveHistory.append(self.currentNode.move)
                    return True
        self.inbook = False
        return False

def getSpace(col: int, row: int, board: list)->int:
        if( row >= 0 and row < 8 and col >= 0 and col < 8 ):
            return board[ col + 8 * row ]
        else:
            return OFFBOARD

def printBoardToTerminal(board: list):
    for i in range(7, -1, -1):
        for j in range(8):
            c = getSpace(j,i, board)
            print(f"{c:3}", end="")
        print()


   
if __name__ == "__main__":
    game = Chess()
    game.mainloop()
    #b = chessbot.getCurrentBoard()
    #chessbot.makeAutomaticMove(1)
    #b = chessbot.getCurrentBoard()
    #printBoardToTerminal(b)