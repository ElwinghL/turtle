#include "turtle-ast.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define PI 3.141592653589793

struct ast_node *make_expr_value(double value) {
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_EXPR_VALUE;
  node->u.value = value;
  return node;
}

struct ast_node *make_cmd_forward(struct ast_node *expr) {
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_CMD_SIMPLE;
  node->u.cmd = CMD_FORWARD;
  node->children_count = 1;
  node->children[0] = expr;
  return node;
}

struct ast_node *make_cmd_backward(struct ast_node *expr) {
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_CMD_SIMPLE;
  node->u.cmd = CMD_BACKWARD;
  node->children_count = 1;
  node->children[0] = expr;
  return node;
}

struct ast_node *make_cmd_position(struct ast_node *abscisse, struct ast_node *ordonnee) {
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_CMD_SIMPLE;
  node->u.cmd = CMD_POSITION;
  node->children_count = 2;
  node->children[0] = abscisse;
  node->children[1] = ordonnee;
  return node;
}

struct ast_node *make_cmd_right(struct ast_node *expr) {
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_CMD_SIMPLE;
  node->u.cmd = CMD_RIGHT;
  node->children_count = 1;
  node->children[0] = expr;
  return node;
}

struct ast_node *make_cmd_left(struct ast_node *expr) {
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_CMD_SIMPLE;
  node->u.cmd = CMD_LEFT;
  node->children_count = 1;
  node->children[0] = expr;
  return node;
}

struct ast_node *make_cmd_heading(struct ast_node *expr) {
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_CMD_SIMPLE;
  node->u.cmd = CMD_HEADING;
  node->children_count = 1;
  node->children[0] = expr;
  return node;
}

struct ast_node *make_cmd_up(){
    struct ast_node *node = calloc(1, sizeof(struct ast_node));
    node->kind = KIND_CMD_SIMPLE;
    node->u.cmd = CMD_UP;
    node->children_count = 0;
    return node;
}
struct ast_node *make_cmd_down(){
    struct ast_node *node = calloc(1, sizeof(struct ast_node));
    node->kind = KIND_CMD_SIMPLE;
    node->u.cmd = CMD_DOWN;
    node->children_count = 0;
    return node;
}
struct ast_node *make_cmd_colors(struct ast_node *color0, struct ast_node *color1, struct ast_node *color2){
    struct ast_node *node = calloc(1, sizeof(struct ast_node));
    node->kind = KIND_CMD_SIMPLE;
    node->u.cmd = CMD_COLOR;
    node->children_count = 3;
    node->children[0] = color0;
    node->children[1] = color1;
    node->children[2] = color2;
    return node;
}

void ast_destroy_r(struct ast_node *self) {
    if (self) {
        if (self->next) {
            ast_destroy_r(self->next);
            for (int i = 0; i < 3; ++i) {
                if (self->children[i]) {
                    ast_destroy_r(self->children[i]);
                }
            }
        }
        free(self);
    }
}

void ast_destroy(struct ast *self) {
  if (self->unit) {
      if (self->unit->next) {
          ast_destroy_r(self->unit->next);
          for (int i = 0; i < 3; ++i) {
              if (self->unit->children[i]) {
                  ast_destroy_r(self->unit->children[i]);
              }
          }
      }
      free(self->unit);
  }
}


/*
 * context
 */

void context_create(struct context *self) {
    self->up = false;
    self->x = 0;
    self->y = 0;
    self->angle = -90;
}

/*
 * eval
 */

void ast_eval(const struct ast *self, struct context *ctx) {
    switch (self->unit->kind) {
        case KIND_CMD_SIMPLE:
            switch (self->unit->u.cmd) {
                case CMD_UP:
                    ctx->up = true;
                    //printf("UP\n");
                    break;
                case CMD_DOWN:
                    ctx->up = false;
                    //printf("DOWN\n");
                    break;
                case CMD_RIGHT:
                    ctx->angle += ast_expr_eval(self->unit->children[0]);
                    break;
                case CMD_LEFT:
                    ctx->angle -= ast_expr_eval(self->unit->children[0]);
                    break;
                case CMD_HEADING:
                    ctx->angle = -90 + ast_expr_eval(self->unit->children[0]);
                    break;
                case CMD_FORWARD:
                    if(ctx->up) {
                        double val = ast_expr_eval(self->unit->children[0]);
                        ctx->x += val * cos(ctx->angle * PI / 180);
                        ctx->y += val * sin(ctx->angle * PI / 180);
                        printf("MoveTo %f %f\n",ctx->x,ctx->y);
                    } else {
                        double val = ast_expr_eval(self->unit->children[0]);
                        ctx->x += val * cos(ctx->angle * PI / 180);
                        ctx->y += val * sin(ctx->angle * PI / 180);
                        printf("LineTo %f %f\n",ctx->x,ctx->y);
                    }
                    break;
                case CMD_BACKWARD:
                    if(ctx->up) {
                        double val = ast_expr_eval(self->unit->children[0]);
                        ctx->x -= val * cos(ctx->angle * PI / 180);
                        ctx->y -= val * sin(ctx->angle * PI / 180);
                        printf("MoveTo %f %f\n",ctx->x,ctx->y);
                    } else {
                        double val = ast_expr_eval(self->unit->children[0]);
                        ctx->x -= val * cos(ctx->angle * PI / 180);
                        ctx->y -= val * sin(ctx->angle * PI / 180);
                        printf("LineTo %f %f\n",ctx->x,ctx->y);
                    }
                    break;
                case CMD_POSITION:
                    ctx->x = ast_expr_eval(self->unit->children[0]);
                    ctx->y = ast_expr_eval(self->unit->children[1]);
                    printf("MoveTo %f %f\n",ctx->x,ctx->y);
                    break;
                case CMD_HOME:
                    ctx->x = 0;
                    ctx->y = 0;
                    printf("MoveTo %f %f\n",ctx->x,ctx->y);
                    break;
                case CMD_COLOR:
                    printf("Color");
                    for (int i = 0; i < 3; ++i) {
                        printf(" %f",ast_expr_eval(self->unit->children[i]));
                    }
                    printf("\n");
                    break;
                case CMD_PRINT:break;
            }
            break;
        case KIND_EXPR_VALUE:
            break;
        case KIND_EXPR_BLOCK:
            break;
        case KIND_CMD_REPEAT:break;
        case KIND_CMD_BLOCK:break;
        case KIND_CMD_PROC:break;
        case KIND_CMD_CALL:break;
        case KIND_CMD_SET:break;
        case KIND_EXPR_FUNC:break;
        case KIND_EXPR_BINOP:break;
        case KIND_EXPR_NAME:break;
    }
    if (self->unit->next) {
        ast_eval_r(self->unit->next, ctx);
    }
}

void ast_eval_r(const struct ast_node *self, struct context *ctx) {
    switch (self->kind) {
        case KIND_CMD_SIMPLE:
            switch (self->u.cmd) {
                case CMD_UP:
                    ctx->up = true;
                    //printf("UP\n");
                    break;
                case CMD_DOWN:
                    ctx->up = false;
                    //printf("DOWN\n");
                    break;
                case CMD_RIGHT:
                    ctx->angle += ast_expr_eval(self->children[0]);
                    break;
                case CMD_LEFT:
                    ctx->angle -= ast_expr_eval(self->children[0]);
                    break;
                case CMD_HEADING:
                    ctx->angle = ast_expr_eval(self->children[0]);
                    break;
                case CMD_FORWARD:
                    if(ctx->up) {
                        double val = ast_expr_eval(self->children[0]);
                        ctx->x += val * cos(ctx->angle * PI / 180);
                        ctx->y += val * sin(ctx->angle * PI / 180);
                        printf("MoveTo %f %f\n",ctx->x,ctx->y);
                    } else {
                        double val = ast_expr_eval(self->children[0]);
                        ctx->x += val * cos(ctx->angle * PI / 180);
                        ctx->y += val * sin(ctx->angle * PI / 180);
                        printf("LineTo %f %f\n",ctx->x,ctx->y);
                    }
                    break;
                case CMD_BACKWARD:
                    if(ctx->up) {
                        double val = ast_expr_eval(self->children[0]);
                        ctx->x -= val * cos(ctx->angle * PI / 180);
                        ctx->y -= val * sin(ctx->angle * PI / 180);
                        printf("MoveTo %f %f\n",ctx->x,ctx->y);
                    } else {
                        double val = ast_expr_eval(self->children[0]);
                        ctx->x -= val * cos(ctx->angle * PI / 180);
                        ctx->y -= val * sin(ctx->angle * PI / 180);
                        printf("LineTo %f %f\n",ctx->x,ctx->y);
                    }
                    break;
                case CMD_POSITION:
                    ctx->x = ast_expr_eval(self->children[0]);
                    ctx->y = ast_expr_eval(self->children[1]);
                    printf("MoveTo %f %f\n",ctx->x,ctx->y);
                    break;
                case CMD_HOME:
                    ctx->x = 0;
                    ctx->y = 0;
                    printf("MoveTo %f %f\n",ctx->x,ctx->y);
                    break;
                case CMD_COLOR:
                    printf("Color");
                    for (int i = 0; i < 3; ++i) {
                        printf(" %f",ast_expr_eval(self->children[i]));
                    }
                    printf("\n");
                    break;
                case CMD_PRINT:break;
            }
            break;
        case KIND_EXPR_VALUE:
            break;
        case KIND_EXPR_BLOCK:
            break;
        case KIND_CMD_REPEAT:break;
        case KIND_CMD_BLOCK:break;
        case KIND_CMD_PROC:break;
        case KIND_CMD_CALL:break;
        case KIND_CMD_SET:break;
        case KIND_EXPR_FUNC:break;
        case KIND_EXPR_BINOP:break;
        case KIND_EXPR_NAME:break;
    }
    if (self->next) {
        ast_eval_r(self->next, ctx);
    }
}


double ast_expr_eval(const struct ast_node *expr) {
    return expr->u.value;
}

/*
 * print
 */

void ast_print(const struct ast *self) {

}