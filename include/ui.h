#ifndef UI_H_
#define UI_H_
#include "NodeManager.h"
#include "setup.h"

extern int SIDEBAR_WIDTH;
void drawEditor(GraphicsContext* context, NodeManager* nodeManager, bool adding, int& uniqueNumber);
void drawSideBar(NodeType* newNodeType);
#endif