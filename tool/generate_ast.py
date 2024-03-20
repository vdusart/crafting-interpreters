# This tool is here to generate a class needed to make the Abstract Syntax Tree (AST)
OUTPUT_DIR = "../jlox/src"
BASE_NAME = "Expr"

class Writer:
    def __init__(self, file):
        self.file = file

    def write(self, text: str, nbIndentations=0):
        identations = "\t" * nbIndentations
        self.file.write(f"{identations}{text}\n")


def defineType(baseName: str, writer: Writer, className: str, fields: str):
    fieldsList: list[str] = fields.split(', ')
    writer.write(f"static class {className} extends {baseName} {{", nbIndentations=1)

    # Fields.
    for field in fieldsList:
        writer.write(f"final {field};", nbIndentations=2)

    # Constructor.
    writer.write('')
    writer.write(f"{className}({fields}) {{", nbIndentations=2)

    # Store parameters in fields.
    for field in fieldsList:
        fieldName = field.split(' ')[1]
        writer.write(f"this.{fieldName} = {fieldName};", nbIndentations=3)
    writer.write("}", nbIndentations=2)

    # Visitor pattern.
    writer.write('')
    writer.write('@Override', nbIndentations=2)
    writer.write("<R> R accept(Visitor<R> visitor) {", nbIndentations=2)
    writer.write(f"return visitor.visit{className}{baseName}(this);", nbIndentations=3)
    writer.write("}", nbIndentations=2)

    writer.write("}", nbIndentations=1)

def defineVistor(baseName: str, writer: Writer, types: list[str]):
    writer.write("interface Visitor<R> {", nbIndentations=1)

    for type in types:
        typeName = type.split(':')[0].strip()
        writer.write(f"R visit{typeName}{baseName}({typeName} {baseName.lower()});", nbIndentations=2)
    writer.write("}\n", nbIndentations=1)

def defineAST(baseName: str, types: list[str]):
    path = f"{OUTPUT_DIR}/{baseName}.java"
    f = open(path, 'w')
    writer = Writer(f)

    writer.write(f"abstract class {baseName} {{")

    defineVistor(baseName, writer, types)

    # The base accept() method.
    writer.write("abstract <R> R accept(Visitor<R> visitor);", nbIndentations=1)
    writer.write('')

    # The AST classes.
    for type in types:
        className = type.split(':')[0].strip()
        fields = type.split(':')[1].strip()
        defineType(baseName, writer, className, fields)

    writer.write('}')
    f.close()

if __name__ == "__main__":
    # ----------- Generating the Expr file -------------------
    types: list[str] = [
        "Binary     : Expr left, Token operator, Expr right",
        "Grouping   : Expr expression",
        "Literal    : Object value",
        "Unary      : Token operator, Expr right",
        "Variable   : Token name"
    ]
    defineAST("Expr", types)
    print("Done generating 'Expr' class.")

    # ----------- Generating the Stmt file -------------------
    types: list[str] = [
        "Expression     : Expr expression",
        "Print          : Expr expression",
        "Var            : Token name, Expr initializer"
    ]
    defineAST("Stmt", types)
    print("Done generating 'Stmt' class.")
