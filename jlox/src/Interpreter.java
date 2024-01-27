class Interpreter implements Expr.Visitor<Object> {

    void interpret(Expr expression) {
        try {
            Object value =  evaluate(expression);
            System.out.println(stringify(value));
        } catch (RuntimeError error) {
            Lox.runtimeError(error);
        }
    }

    @Override
    public Object visitLiteralExpr(Expr.Literal expr) {
        return expr.value;
    }

    @Override
    public Object visitGroupingExpr(Expr.Grouping expr) {
        return evaluate(expr.expression);
    }

    @Override
    public Object visitUnaryExpr(Expr.Unary expr) {
        Object right = evaluate(expr.right);

        switch (expr.operator.type) {
            case TokenType.BANG:
                return !isTruthy(right);
            case TokenType.MINUS:
                checkNumberOperand(expr.operator, right);
                return -(double)right;
        }

        // Unreachable
        return null;
    }

    private void checkNumberOperand(Token operator, Object operand) {
        if (operand instanceof Double) return;
        throw new RuntimeError(operator, "Operand must be a number.");
    }

    private void checkNumberOperands(Token operator, Object left, Object right) {
        if (left instanceof Double && right instanceof Double) return;
        throw new RuntimeError(operator, "Operands must be numbers.");
    }

    private void checkNumberOrStringOperands(Token operator, Object left, Object right) {
        if (left instanceof Double && right instanceof Double) return;
        if (left instanceof String && right instanceof String) return;
        throw new RuntimeError(operator, "Operands must be 2 numbers or 2 strings.");
    }

    @Override
    public Object visitBinaryExpr(Expr.Binary expr) {
        Object left = evaluate(expr.left);
        Object right = evaluate(expr.right);

        switch (expr.operator.type) {
            case TokenType.BANG_EQUAL: return !isEqual(left, right);
            case TokenType.EQUAL_EQUAL: return isEqual(left, right);
            case TokenType.GREATER:
                if (left instanceof Double && right instanceof Double)
                    return (double)left > (double)right;
                if (left instanceof String && right instanceof String) {
                    int comparison = stringify(left).compareTo(stringify(right));
                    return comparison > 0;
                }
                throw new RuntimeError(expr.operator, "Operands must be 2 numbers or 2 strings.");
            case TokenType.GREATER_EQUAL:
                if (left instanceof Double && right instanceof Double)
                    return (double)left >= (double)right;
                if (left instanceof String && right instanceof String) {
                    int comparison = stringify(left).compareTo(stringify(right));
                    return comparison >= 0;
                }
                throw new RuntimeError(expr.operator, "Operands must be 2 numbers or 2 strings.");
            case TokenType.LESS:
                if (left instanceof Double && right instanceof Double)
                    return (double)left < (double)right;
                if (left instanceof String && right instanceof String) {
                    int comparison = stringify(left).compareTo(stringify(right));
                    return comparison < 0;
                }
                throw new RuntimeError(expr.operator, "Operands must be 2 numbers or 2 strings.");
            case TokenType.LESS_EQUAL:
                if (left instanceof Double && right instanceof Double)
                    return (double)left <= (double)right;
                if (left instanceof String && right instanceof String) {
                    int comparison = stringify(left).compareTo(stringify(right));
                    return comparison <= 0;
                }
                throw new RuntimeError(expr.operator, "Operands must be 2 numbers or 2 strings.");

            case TokenType.MINUS:
                checkNumberOperands(expr.operator, left, right);
                return (double)left - (double)right;
            case TokenType.PLUS:
                if (left instanceof Double && right instanceof Double){
                    return (double)left + (double)right;
                }
                if (left instanceof String || right instanceof String) {
                    return stringify(left) + stringify(right);
                }
                throw new RuntimeError(expr.operator, "Operands must be two numbers or one of them must be a string.");
            case TokenType.SLASH:
                checkNumberOperands(expr.operator, left, right);
                if ((double)right == 0) throw new RuntimeError(expr.operator, "Cannot divide by 0.");
                return (double)left / (double)right;
            case TokenType.STAR:
                checkNumberOperands(expr.operator, left, right);
                return (double)left * (double)right;
        }
        return null;
    }

    private boolean isTruthy(Object object) {
        if (object == null) return false;
        if (object instanceof Boolean) return (boolean)object;
        return true;
    }

    private boolean isEqual(Object a, Object b) {
        if (a == null && b == null) return true;
        if (a == null) return false;
        return a.equals(b);
    }

    private String stringify(Object object) {
        if (object == null) return "nil";
        if (object instanceof Double) {
            String text = object.toString();
            if (text.endsWith(".0")) {
                text = text.substring(0, text.length() - 2);
            }
            return text;
        }

        return object.toString();
    }

    private Object evaluate(Expr expr) {
        return expr.accept(this);
    }
}
