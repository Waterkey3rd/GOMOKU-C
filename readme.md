系统采用前端后端分离架构，前后端用接口进行通信。使用前后端分离架构，可以解耦逻辑。让前端更注重于显示逻辑，后端更注重于数据处理，日志处理.
系统架构设计
前端
暂定，先写完后端，在考虑前端逻辑
非图形:cmd
图形:Dear ImGui
后端
线性数据表结构（选择）：
1.二维数组；2.二进制存储
功能分析
1.棋盘落子接口
2.检测胜利
3.存储局面的总手数，下一手颜色信息
4.棋盘清空
5.绑定选手
6.考虑ai接入的方便性（就是加落子接口和数据返回信息结构化）
7.logs日志
class设计

Chessboard
接口设计：
1. reset_board() 初始化/重置棋盘。将所有格点设为空状态。
2. place_piece(x, y, color) 落子接口。在坐标 $$(x,y)$$ 放置颜色为 color 的棋子。返回操作是否成功（如该点已有子则返回失败）
3. get_piece_at(x, y) 获取指定坐标的棋子状态（黑、白或空）
4. is_full() 检查棋盘是否已满（用于判定平局）
5. get_last_move() 返回最后一手棋的坐标（用于 UI 高亮标记）
6. check_winner(x, y)  综合判断游戏是否结束（胜负或平局）
7. Update() 进行棋盘状态自动检查更新
class Chessboard{
private:
    size_t H,W;
    size_t logsL;
    unit8_t board[H][W];
    std::array history[logsL];
};
player
作用：实现ai和人类选手逻辑统一，设计统一接口，通过派生类实现动态多态。
属性接口设计：
1. Color 用来标识颜色
2. getColor () 获取颜色
3. commitMove() 提交落子请求，把提交数据暂存在类中
4. comfirmSelection（）确认请求，真正把数据发到chessboard
class Player{
}
