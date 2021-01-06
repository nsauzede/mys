from ..parser import ast


class BodyCheckVisitor(ast.NodeVisitor):

    def visit_Expr(self, node):
        if isinstance(node.value, ast.Name):
            raise CompileError("bare name", node)
        elif isinstance(node.value, ast.Compare):
            raise CompileError("bare comparision", node)
        elif isinstance(node.value, ast.BinOp):
            raise CompileError("bare binary operation", node)
        elif isinstance(node.value, ast.UnaryOp):
            raise CompileError("bare unary operation", node)
        elif isinstance(node.value, ast.Constant):
            if isinstance(node.value.value, str):
                # ToDo: embedded C++
                pass
            elif isinstance(node.value.value, int):
                raise CompileError("bare integer", node)
            if isinstance(node.value.value, float):
                raise CompileError("bare float", node)