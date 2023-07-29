package lib.tools.exceptions;

import lib.symbolTable.Symbol;

public class WrongAttrTypeException extends Exception {
    private Symbol.Types given, expected;

    public WrongAttrTypeException(Symbol.Types given, Symbol.Types expected) {
        this.given = given;
        this.expected = expected;
    }

    public String givenType() {
        switch (given) {
            case INT: return "Integer";
            case CHAR: return "Char";
            case BOOL: return "Bool";
            case STRING: return "String";
            default : return "";
        }
    }

    public String expectedType() {
        switch (expected) {
            case INT: return "Integer";
            case CHAR: return "Char";
            case BOOL: return "Bool";
            case STRING: return "String";
            default : return "";
        }
    }
}
