import wrappers

class CppWrapperEnum():
    name: str
    value: int

    _item_to_string: dict[int, str]
    _string_to_item: dict[str, int]
        
    def __init__(self, _cppEnumName: str, value_or_name: int | str):
        self._name: str = _cppEnumName
        cpp_enum = getattr(wrappers, _cppEnumName)

        self.wrapper_items = cpp_enum.__members__.values()
        self._item_to_string = {item.value: item.name for item in self.wrapper_items}
        self._string_to_item = {item.name: item.value for item in self.wrapper_items}

        if isinstance(value_or_name, str):
            self.name = value_or_name
            self.value = self.name_to_value(value_or_name)
        elif isinstance(value_or_name, int):
            self.value = value_or_name
            self.name = self.value_to_name(value_or_name)
        elif isinstance(value_or_name, cpp_enum):
            self.name = value_or_name.name
            self.value = value_or_name.value
        else:
            raise ValueError(f"Invalid input for {self._name}")

    def value_to_name(self, value: int):
        if value not in self._item_to_string:
            raise ValueError(f"Invalid {self._name} value: {value}")
        return self._item_to_string[value]
    
    def name_to_value(self, name: str):
        if name not in self._string_to_item:
            raise ValueError(f"Invalid {self._name} name: {name}")
        return self._string_to_item[name]
    
    def to_cpp_obj(self):
        cpp_enum = getattr(wrappers, self._name)
        return cpp_enum(self.value)
    
    def __eq__(self, other) -> bool:
        if isinstance(other, int):
            return self.value == other
        elif isinstance(other, str):
            return self.name == other
        elif isinstance(other, CppWrapperEnum) and self._name == other._name:
            return self.value == other.value
        else:
            return False
    
    def __str__(self):
        return self.name
    
    def __repr__(self) -> str:
        return f"{self._name}.{self.name}"


class Square(CppWrapperEnum):
    def __init__(self, value_or_name: int | str):
        super().__init__('Square', value_or_name)


class Piecetype(CppWrapperEnum):
    def __init__(self, value_or_name: int | str):
        super().__init__('Piecetype', value_or_name)
    
    def to_cpp_object(self):
        return wrappers.string_piecetype_map()[self.name.lower()]
    

class Color(CppWrapperEnum):
    def __init__(self, value_or_name: int | str):
        super().__init__('Color', value_or_name)


class GameResult(CppWrapperEnum):
    def __init__(self, value_or_name: int | str):
        super().__init__('GameResult', value_or_name)


class PlayerType(CppWrapperEnum):
    def __init__(self, value_or_name: int | str):
        super().__init__('PlayerType', value_or_name)


class Move:
    playerPiece     : wrappers.Piecetype
    fromSquare      : wrappers.Square
    targetSquare    : wrappers.Square
    isCastling      : bool = False
    isCapture       : bool = False
    isPromotion     : bool = False
    promotionPiece  : wrappers.Piecetype    = wrappers.Piecetype.NoType
    capturePiece    : wrappers.Piecetype    = wrappers.Piecetype.NoType
    captureSquare   : wrappers.Square       = wrappers.Square.NoSquare
    newEnPassant    : wrappers.Square       = wrappers.Square.NoSquare

    # Move from parts
    def __init__(self,
            playerPiece         : wrappers.Piecetype,
            fromSquare          : wrappers.Square,
            targetSquare        : wrappers.Square, 
            isCastling          : bool                = False,
            isCapture           : bool                = False,
            isPromotion         : bool                = False,
            promotionPiece      : wrappers.Piecetype  = wrappers.Piecetype.NoType,
            capturePiece        : wrappers.Piecetype  = wrappers.Piecetype.NoType,
            captureSquare       : wrappers.Square     = wrappers.Square.NoSquare,
            newEnPassant        : wrappers.Square     = wrappers.Square.NoSquare
        ):
        self.playerPiece        = playerPiece
        self.fromSquare         = fromSquare
        self.targetSquare       = targetSquare
        self.isCastling         = isCastling
        self.isCapture          = isCapture
        self.isPromotion        = isPromotion
        self.promotionPiece     = promotionPiece
        self.capturePiece       = capturePiece
        self.captureSquare      = captureSquare
        self.newEnPassant       = newEnPassant

    # Move from wrappers object
    def __init__(self, move: wrappers.Move):
        self.playerPiece        = move.playerPiece
        self.fromSquare         = move.fromSquare
        self.targetSquare       = move.targetSquare
        self.isCastling         = move.isCastling
        self.isCapture          = move.isCapture
        self.isPromotion        = move.isPromotion
        self.promotionPiece     = move.promotionPiece
        self.capturePiece       = move.capturePiece
        self.captureSquare      = move.captureSquare
        self.newEnPassant       = move.newEnPassant

    def __eq__(self, other):
        if not isinstance(other, Move):
            raise NotImplementedError
        return ((self.fromSquare == other.fromSquare) &
                (self.targetSquare == other.targetSquare) &
                (self.promotionPiece == other.promotionPiece))
    
    def __repr__(self):
        return f"{self.fromSquare.name}{self.targetSquare.name} - {self.playerPiece.name}"
