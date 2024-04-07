import wrappers
from enum import Enum


class Move:
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


class Square:
    squares = wrappers.Square.__members__.values()
    _Square = Enum('Square', {square.name: square.value for square in squares})
    _square_to_string = {square.value: square.name for square in squares}
    _string_to_square = {square.name: square.value for square in squares}

    value: int
    name: str

    def __init__(self, value_or_name):
        if isinstance(value_or_name, str):
            self.name = value_or_name
            self.value = self.name_to_value(value_or_name)
        elif isinstance(value_or_name, int):
            self.value = value_or_name
            self.name = self.value_to_name(value_or_name)
        elif isinstance(value_or_name, wrappers.Square):
            self.name = value_or_name.name
            self.value = value_or_name.value
        else:
            raise ValueError("Invalid input for Square")

    def __eq__(self, other) -> bool:
        if isinstance(other, int):
            return self.value == other
        elif not isinstance(other, Square):
            raise NotImplementedError(f"Dont compare {type(self).__name__} with {type(other).__name__}")
        return self.value == other.value

    def __repr__(self) -> str:
        return self.name

    @staticmethod
    def value_to_name(value: int):
        if value not in Square._square_to_string:
            raise ValueError(f"Invalid square value: {value}")
        return Square._square_to_string[value]
    
    @staticmethod
    def name_to_value(name: str):
        if name not in Square._string_to_square:
            raise ValueError(f"Invalid square name: {name}")
        return Square._string_to_square[name]
    
    
class Piecetype:
    piecetypes = wrappers.Piecetype.__members__.values()
    _Piecetype = Enum('Piecetype', {piecetype.name: piecetype.value for piecetype in piecetypes})
    _piecetype_to_string = {piecetype.value: piecetype.name for piecetype in piecetypes}
    _string_to_piecetype = {piecetype.name: piecetype.value for piecetype in piecetypes}

    name: str
    value: int

    def __init__(self, value_or_name):
        if isinstance(value_or_name, str):
            self.name = value_or_name
            self.value = self.name_to_value(value_or_name)
        elif isinstance(value_or_name, int):
            self.value = value_or_name
            self.name = self.value_to_name(value_or_name)
        elif isinstance(value_or_name, wrappers.Piecetype): 
            self.value = value_or_name.value
            self.name = value_or_name.name
        else:
            raise ValueError("Invalid input for Piecetype")
    
    def to_cpp_object(self):
        return wrappers.string_piecetype_map()[self.name.lower()]

    def __eq__(self, other) -> bool:
        if isinstance(other, int):
            return self.value == other
        elif not isinstance(other, Square):
            raise NotImplementedError(f"Dont compare {type(self).__name__} with {type(other).__name__}")
        return self.value == other.value

    def __repr__(self) -> str:
        return self.name

    @staticmethod
    def value_to_name(value: int):
        if value not in Piecetype._piecetype_to_string:
            raise ValueError(f"Invalid piecetype value: {value}")
        return Piecetype._piecetype_to_string[value]
    
    @staticmethod
    def name_to_value(name: str):
        if name not in Piecetype._string_to_piecetype:
            raise ValueError(f"Invalid piecetype name: {name}")
        return Piecetype._string_to_piecetype[name]
    

class Color:
    colors = wrappers.Color.__members__.values()
    _Color = Enum('Color', {color.name: color.value for color in colors})
    _color_to_string = {color.value: color.name for color in colors}
    _string_to_color = {color.name: color.value for color in colors}

    name: str
    value: int
    
    def __init__(self, value_or_name):
        if isinstance(value_or_name, str):
            self.name = value_or_name
            self.value = self.name_to_value(value_or_name)
        elif isinstance(value_or_name, int):
            self.value = value_or_name
            self.name = self.value_to_name(value_or_name)
        elif isinstance(value_or_name, wrappers.Color):
            self.name = value_or_name.name
            self.value = value_or_name.value
        else:
            raise ValueError("Invalid input for Color")
    
    def __eq__(self, other) -> bool:
        if isinstance(other, int):
            return self.value == other
        elif not isinstance(other, Color):
            raise NotImplementedError(f"Dont compare {type(self)} with {type(other)}")
        return self.value == other.value

    def __repr__(self) -> str:
        return self.name

    @staticmethod
    def value_to_name(value: int):
        if value not in Color._color_to_string:
            raise ValueError(f"Invalid color value: {value}")
        return Color._color_to_string[value]
    
    @staticmethod
    def name_to_value(name: str):
        if name not in Color._string_to_color:
            raise ValueError(f"Invalid color name: {name}")
        return Color._string_to_color[name]


class GameResult:
    gameResults = wrappers.GameResult.__members__.values()
    _GameResult = Enum('GameResult', {result.name: result.value for result in gameResults})
    _result_to_string = {result.value: result.name for result in gameResults}
    _string_to_result = {result.name: result.value for result in gameResults}

    name: str
    value: int
    
    def __init__(self, value_or_name):
        if isinstance(value_or_name, str):
            self.name = value_or_name
            self.value = self.name_to_value(value_or_name)
        elif isinstance(value_or_name, int):
            self.value = value_or_name
            self.name = self.value_to_name(value_or_name)
        elif isinstance(value_or_name, wrappers.GameResult):
            self.name = value_or_name.name
            self.value = value_or_name.value
        else:
            raise ValueError(f"Invalid input for GameResult: {value_or_name}")
    
    def __eq__(self, other) -> bool:
        if isinstance(other, int):
            return self.value == other
        elif not isinstance(other, GameResult):
            raise NotImplementedError(f"Dont compare {type(self)} with {type(other)}")
        return self.value == other.value

    def __repr__(self) -> str:
        return self.name

    @staticmethod
    def value_to_name(value: int):
        if value not in GameResult._result_to_string:
            raise ValueError(f"Invalid result value: {value}")
        return GameResult._result_to_string[value]
    
    @staticmethod
    def name_to_value(name: str):
        if name not in GameResult._string_to_result:
            raise ValueError(f"Invalid result name: {name}")
        return GameResult._string_to_result[name]


class PlayerType:
    playerTypes = wrappers.PlayerType.__members__.values()
    _PlayerTypes = Enum('PlayerTypes', {player_type.name: player_type.value for player_type in playerTypes})
    _result_to_string = {player_type.value: player_type.name for player_type in playerTypes}
    _string_to_result = {player_type.name: player_type.value for player_type in playerTypes}

    name: str
    value: int
    
    def __init__(self, value_or_name):
        if isinstance(value_or_name, str):
            self.name = value_or_name
            self.value = self.name_to_value(value_or_name)
        elif isinstance(value_or_name, int):
            self.value = value_or_name
            self.name = self.value_to_name(value_or_name)
        elif isinstance(value_or_name, wrappers.PlayerType):
            self.name = value_or_name.name
            self.value = value_or_name.value
        else:
            raise ValueError(f"Invalid input for PlayerType: {value_or_name}")
    
    def __eq__(self, other) -> bool:
        if isinstance(other, int):
            return self.value == other
        elif not isinstance(other, PlayerType):
            raise NotImplementedError(f"Dont compare {type(self)} with {type(other)}")
        return self.value == other.value

    def __repr__(self) -> str:
        return self.name
    
    @staticmethod
    def value_to_name(value: int):
        if value not in PlayerType._result_to_string:
            raise ValueError(f"Invalid result value: {value}")
        return PlayerType._result_to_string[value]
    
    @staticmethod
    def name_to_value(name: str):
        name = name[0].upper() + name[1:]
        if name not in PlayerType._string_to_result:
            raise ValueError(f"Invalid result name: {name}")
        return PlayerType._string_to_result[name]
