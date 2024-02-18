use serde::Deserialize;
pub mod helper;
pub mod kind;
pub mod loc;
pub mod visitor;
pub use helper::*;
pub use kind::*;
pub use visitor::*;

pub type Node = clang_ast::Node<Clang>;

/// The AST elements we need to extract and process.
#[derive(Deserialize, Debug, Clone)]
#[serde(deny_unknown_fields)]
pub enum Clang {
    TranslationUnitDecl,
    DeclStmt(DeclStmt),
    ReturnStmt(ReturnStmt),
    CaseStmt,
    DefaultStmt,
    ForStmt,
    BreakStmt,
    ContinueStmt,
    SwitchStmt,
    WhileStmt,
    DoStmt,
    CompoundLiteralExpr,
    ExprWithCleanups,
    MaterializeTemporaryExpr,
    CompoundStmt(CompoundStmt),
    ArraySubscriptExpr(ArraySubscriptExpr),
    ParenExpr(ParenExpr),
    FunctionDecl(FunctionDecl),
    VarDecl(VarDecl),
    QualType(QualType),
    CallExpr(CallExpr),
    ImplicitCastExpr(ImplicitCastExpr),
    CStyleCastExpr(CStyleCastExpr),
    DeclRefExpr(DeclRefExpr),
    StringLiteral(StringLiteral),
    CharacterLiteral(CharacterLiteral),
    FloatingLiteral(FloatingLiteral),
    IntegerLiteral(IntegerLiteral),
    IfStmt(IfStmt),
    ParmVarDecl(ParmVarDecl),
    PointerType(PointerType),
    RecordDecl(RecordDecl),
    RecordType(RecordType),
    BinaryOperator(BinaryOperator),
    UnaryOperator(UnaryOperator),
    LabelStmt(LabelStmt),
    GotoStmt(GotoStmt),
    MemberExpr(MemberExpr),
    InitListExpr(InitListExpr),
    UnaryExprOrTypeTraitExpr(UnaryExprOrTypeTraitExpr),
    /// CXX compatiablity
    CXXNullPtrLiteralExpr(CXXNullPtrLiteralExpr),
    CXXConstructExpr(CXXConstructExpr),
    CXXMemberCallExpr,
    GNUNullExpr(GNUNullExpr),
    CXXReinterpretCastExpr,
    CXXNewExpr(CXXNewExpr),
    CXXStaticCastExpr(CXXStaticCastExpr),
    /// used to extract type definition
    TypedefDecl(TypedefDecl),
    EnumDecl(EnumDecl),
    EnumConstantDecl(EnumConstantDecl),
    ConstantExpr(ConstantExpr),
    FieldDecl(FieldDecl),
    LinkageSpecDecl(LinkageSpecDecl),
    CXXRecordDecl(CXXRecordDecl),
    ElaboratedType(ElaboratedType),
    CXXDefaultArgExpr,
    Other,
}
