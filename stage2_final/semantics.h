/* LAVANYA SONI - 2017A7PS0151P
    KUSHAGRA AGRAWAL - 2017A7PS0107P
    SIDDHANT KHANDELWAL - 2017A7PS0127P
    GROUP - 11
*/

#include "ast.h"

void checkSemantics(tNode *head);
type_info *expressionChecker(tNode *head);
type_info *ioStmtChecker(tNode *head);
type_info *assignmentChecker(tNode *head);
type_info *arrayChecker(tNode *head);

extern int semanticErrors;