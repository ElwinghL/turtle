#ifndef TURTLE_AST_H
#define TURTLE_AST_H

#include <stddef.h>
#include <stdbool.h>

// simple commands
enum ast_cmd {
  CMD_UP,
  CMD_DOWN,
  CMD_RIGHT,
  CMD_LEFT,
  CMD_HEADING,
  CMD_FORWARD,
  CMD_BACKWARD,
  CMD_POSITION,
  CMD_HOME,
  CMD_COLOR,
  CMD_PRINT,
};

// internal functions
enum ast_func {
  FUNC_COS,
  FUNC_RANDOM,
  FUNC_SIN,
  FUNC_SQRT,
};

// kind of a node in the abstract syntax tree
enum ast_kind {
  KIND_CMD_SIMPLE,
  KIND_CMD_REPEAT,
  KIND_CMD_BLOCK,
  KIND_CMD_PROC,
  KIND_CMD_CALL,
  KIND_CMD_SET,

  KIND_EXPR_FUNC,
  KIND_EXPR_VALUE,
  KIND_EXPR_BINOP,
  KIND_EXPR_BLOCK,
  KIND_EXPR_NAME,
};

#define AST_CHILDREN_MAX 3

// a node in the abstract syntax tree
struct ast_node {
    enum ast_kind kind; // kind of the node

    union {
        enum ast_cmd cmd;   // kind == KIND_CMD_SIMPLE
        double value;       // kind == KIND_EXPR_VALUE, for literals
        char op;            // kind == KIND_EXPR_BINOP, for operators in expressions
        const char *name;   // kind == KIND_EXPR_NAME, the name of procedures and variables
        enum ast_func func; // kind == KIND_EXPR_FUNC, a function
    } u;

    size_t children_count;  // the number of children of the node
    struct ast_node *children[AST_CHILDREN_MAX];  // the children of the node (arguments of commands, etc)
    struct ast_node *next;  // the next node in the sequence
};

  struct ast_node *make_cmd_forward(struct ast_node *expr);
  struct ast_node *make_cmd_backward(struct ast_node *expr);
  struct ast_node *make_cmd_position(struct ast_node *abscisse, struct ast_node *ordonee);
  struct ast_node *make_cmd_right(struct ast_node *expr);
  struct ast_node *make_cmd_left(struct ast_node *expr);
  struct ast_node *make_cmd_heading(struct ast_node *expr);
  struct ast_node *make_cmd_up();
  struct ast_node *make_cmd_down();
  struct ast_node *make_cmd_colors(struct ast_node *color0, struct ast_node *color1, struct ast_node *color2);




// TODO: make some constructors to use in parser.y
// for example:
struct ast_node *make_expr_value(double value);


// root of the abstract syntax tree
struct ast {
  struct ast_node *unit;
};

// do not forget to destroy properly! no leaks allowed!
void ast_destroy(struct ast *self);

// the execution context
struct context {
  double x;
  double y;
  double angle;
  bool up;

  // TODO: add procedure handling
  // TODO: add variable handling
};

// create an initial context
void context_create(struct context *self);

// print the tree as if it was a Turtle program
void ast_print(const struct ast *self);

// evaluate the tree and generate some basic primitives
void ast_eval(const struct ast *self, struct context *ctx);
void ast_eval_r(const struct ast_node *self, struct context *ctx);
// evaluate an expression
double ast_expr_eval(const struct ast_node *expr);

#endif /* TURTLE_AST_H */
