#include "ast.h"

void checkSemantics(tNode* head);
type_info* expressionChecker(tNode* head);
type_info* ioStmtChecker(tNode* head);
type_info* assignmentChecker(tNode* head);
type_info* arrayChecker(tNode* head);