import wrappers
from enum import Enum


squares = wrappers.Square.__members__.values()
_Square = Enum('Square', {square.name: square.value for square in squares})
_square_to_string = {square.value: square.name for square in squares}

piecetypes = wrappers.Piecetype.__members__.values()
_Piecetype = Enum('Piecetype', {piecetype.name: piecetype.value for piecetype in piecetypes})
_piecetype_to_string = {piecetype.value: piecetype.name for piecetype in piecetypes}

colors = wrappers.Color.__members__.values()
_Color = Enum('Color', {color.name: color.value for color in colors})
_color_to_string = {color.value: color.name for color in colors}


class Move():
    playerPiece: wrappers.Piecetype
    fromSquare: wrappers.Square
    targetSquare: wrappers.Square
    isCastling: bool = False
    isCapture: bool = False
    isPromotion: bool = False
    promotionPiece: wrappers.Piecetype = wrappers.Piecetype.NoType
    capturePiece: wrappers.Piecetype = wrappers.Piecetype.NoType
    captureSquare: wrappers.Square = wrappers.Square.NoSquare
    newEnPassant: wrappers.Square = wrappers.Square.NoSquare

    # Move from parts
    def __init__(self,
            playerPiece: wrappers.Piecetype,
            fromSquare: wrappers.Square,
            targetSquare: wrappers.Square, 
            isCastling: bool = False,
            isCapture: bool = False,
            isPromotion: bool = False,
            promotionPiece: wrappers.Piecetype = wrappers.Piecetype.NoType,
            capturePiece: wrappers.Piecetype = wrappers.Piecetype.NoType,
            captureSquare: wrappers.Square = wrappers.Square.NoSquare,
            newEnPassant: wrappers.Square = wrappers.Square.NoSquare
        ):
        self.playerPiece = playerPiece
        self.fromSquare = fromSquare
        self.targetSquare = targetSquare
        self.isCastling = isCastling
        self.isCapture = isCapture
        self.isPromotion = isPromotion
        self.promotionPiece = promotionPiece
        self.capturePiece = capturePiece
        self.captureSquare = captureSquare
        self.newEnPassant = newEnPassant

    # Move from wrappers object
    def __init__(self, move: wrappers.Move):
        self.playerPiece = move.playerPiece
        self.fromSquare = move.fromSquare
        self.targetSquare = move.targetSquare
        self.isCastling = move.isCastling
        self.isCapture = move.isCapture
        self.isPromotion = move.isPromotion
        self.promotionPiece = move.promotionPiece
        self.capturePiece = move.capturePiece
        self.captureSquare = move.captureSquare
        self.newEnPassant = move.newEnPassant

    def __eq__(self, other):
        if not isinstance(other, Move):
            raise NotImplementedError
        return ((self.fromSquare == other.fromSquare) &
                (self.targetSquare == other.targetSquare) &
                (self.promotionPiece == other.promotionPiece))
    
    def __repr__(self):
        return f"{self.fromSquare.name}{self.targetSquare.name} - {self.playerPiece.name}"

class Square():
    value: int
    name: str
    
    def __init__(self, name: str, value: int):
        self.name = name
        self.value = value
    
    def __init__(self, value: int):
        assert value in _square_to_string, "Square invalid"
        self.name = _square_to_string[value]
        self.value = value

    def __eq__(self, other) -> bool:
        if isinstance(other, int):
            return self.value == other
        elif not isinstance(other, Square):
            raise NotImplementedError(f"Dont compare {type(self).__name__} with {type(other).__name__}")
        return self.value == other.value

    def __repr__(self) -> str:
        return self.name
    
class Piecetype:
    name: str
    value: int

    def __init__(self, name: str, value: int):
        self.name = name
        self.value = value
    
    def __init__(self, piecetype: wrappers.Piecetype):
        self.value = piecetype.value
        self.name = piecetype.name

    def __eq__(self, other) -> bool:
        if isinstance(other, int):
            return self.value == other
        elif not isinstance(other, Square):
            raise NotImplementedError(f"Dont compare {type(self).__name__} with {type(other).__name__}")
        return self.value == other.value

    def __repr__(self) -> str:
        return self.name

class Color:
    name: str
    value: int

    def __init__(self, name: str, value: int):
        self.name = name
        self.value = value
    
    def __init__(self, color: wrappers.Color):
        self.value = color.value
        self.name = color.name
    
    def __eq__(self, other) -> bool:
        if not isinstance(other, Color):
            raise NotImplementedError
        return self.value == other.value

    def __repr__(self) -> str:
        return self.name

