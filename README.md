# 火车票管理系统

> CS147 group assignment, ACM Honors Class, SJTU

火车票管理系统为组队作业，共15组，每组由两人组成。

| **Team name** : 龙龙子\_official_新搬运 |
| :-------------------------------------: |

|                   Teammate                    |        Mission        |
| :-------------------------------------------: | :-------------------: |
| [andy-yang-1](https://github.com/andy-yang-1) |    Main procedure｜Web BackEnd|    |
|   [happypig](https://github.com/happierpig)   | DataStructure｜Web FrontEnd |

## 概况

#### 组队要求

- A班同学必须和B班同学组队完成，即不可有A班同学一起组队的情况，此处A班同学包括上课的同学和队爷。
- 组好队后请在[石墨文档](https://shimo.im/sheets/hpjTx6kgdCkR9jyK/MODOC)中登记。
- 想要solo的请联系助教。

#### 实现内容

本次作业要求实现一个类似于12306的火车票订票系统，该系统能将用户数据、购票数据、车次数据进行**本地存储**，并对其进行高效操作。


#### 程序结构

后端程序需要实现文档中要求的接口，在接受完整命令后立即输出结果并再次进入响应输入的状态。使用C++实现。

可以使用的库为：`iostream`、`cstdio`、`cstring`、`cmath`、`fstream`、`stringstream`、`functional`。

如需要使用其他库，可以向助教申请。

## 评分标准

#### A. 基础分 - 85%

- 性能 - 75%

  在所有测试中，内存使用均会被严格限制。测试过程中程序可能会被多次打开关闭。
  
  你可以在可执行文件所在目录创建不超过20个文件。

  - 正确性测试 - 50%

    在正确性测试中，每一个测试点都有一个相对宽松的时间和磁盘使用限制，以仅检验程序的正确性和鲁棒性，只要通过测试即可得到满分。因此请不要尝试针对特定情况进行有损正确性和鲁棒性的优化。
    
  - 压力测试 - 25%

- 文档 - 10%

  文档方面要求提供完整的**《开发文档》**(包括模块划分图、每个模块的功能、类设计、文件设计等)以及**各个组员工作情况**的说明。

#### B. bonus - 15%

- 后端

  推荐学习数据库相关算法，或是在不影响正常功能的情况下，添加其他特性。

  可供选择的bonus及对应分数为：

  - 添加缓存（推荐）

    - 对于一棵B+树，有一些节点我们会进行反复的读取，比如根节点、根节点的子节点，以及一些经常访问到的数据等。我们如果把部分这样的数据放在内存中，就可以提升程序的运行速度（当然，你不能把整颗B+树放在内存中，因为我们会限制你使用的内存大小）。
    - 分数：5%~8% （根据缓存实现难度给分）

  - B+树的空间回收实现（推荐）

    - 在删除节点后，能否把那个删除掉的节点的空间利用起来呢？

    - 分数：3%

  - 并发

    - 中国的12306火车票系统以变态的高并发性能闻名。所谓高并发，通俗来说就是同时接收处理许多来自客户的请求。当代家用和服务器CPU核心都很多，可以同时运行很多线程，在本次大作业中我们鼓励大家对后端程序主逻辑以及B+树运用多线程优化。对于做并发bonus的同学，鼓励大家自行设计一些测试来证明自己运行效率的优势，通过code review进行展示。
    - **尝试此bonus的同学请务必与助教联系并讨论自己的实现思路。**
    - 分数：最高15%

  - 数据库的/导出/备份/增量备份/回滚

    - 对于一个强大的数据库系统来说，支持备份与恢复功能是非常重要的。这极大提升了你的数据库的安全性，如果一个数据库拥有了定时自动备份的功能，就算被误操作kill掉了，仍旧可以恢复到最近的一次备份状态。
      第一次备份必须是完全的备份。接下来的备份可以是完全备份，或者是增量备份，这取决于你对安全和性能的平衡。增量备份可以支持恢复到任何一个之前的版本。
    - 同时，你可以用你的备份功能来支持数据库的迁移。在另一台全新的机器上，只要拥有了你的可执行程序和导出的数据包，就可以继续该数据库之前的执行状态。
    - **尝试此bonus的同学请务必与助教联系并讨论自己的实现思路。**
    - 分数：最高10%

  - 想要做其他bonus的可以联系助教。

- 前端

  提供一个用户友好的**图形操作页面**，推荐实现跨平台的前端，如网页，Qt等。基本要求为：

  - 前后端分离
  - 用户能通过用户友好的图形界面完成后端接口提供的任何操作，并且能够正常地通过网络实现前后端通信
  - 提供完整的**《使用手册》**(包括系统安装手册、用户手册等)
  - **尝试此bonus的同学请务必与助教联系。**
  - 分数：最高15%

最终bonus分数为所有bonus评分之和与15%中取较低者，分数不会溢出。

#### C. 扣分

- 每位小组成员都需要有绝对参与度，不要求相对参与度，小组内自己协商工作。我们建议有两位成员的小组，一位同学写B+树主体，一位同学写后端主体逻辑部分，若小组内不是这样的分工，请联系助教说明分工情况。若小组内某位组员参与度过低，会扣除一定分数，情况恶劣者扣分无上限。
- 请保证自己项目结构的可读性，可以包括优化项目结构、完善README的内容、适当的文件树指南等，晦涩难懂的项目可能会加大助教的工作量，也可能会影响你的成绩。

## 接口说明

#### 用户相关

一个用户有以下数据：

- `username`：用户的唯一标识符，由字母开头，字母、数字和下划线组成的字符串，长度不超过20。
- `password`：由字母、数字和下划线组成的字符串，长度不低于6，不超过30。
- `name`：由二至五个汉字组成的字符串。
- `mailAddr`：同一般邮箱地址格式，且仅含数字、大小写字母，`@`和`.`，长度不超过30。（无需检验邮箱格式的合法性）
- `privilege`：所处用户组优先级，为一个0~10的整数。

#### 车次相关

一个车次有以下属性：

- `trainID`：车次的唯一标识符，由字母开头，字母、数字和下划线组成的字符串，长度不超过20。

- `stationNum`：车次经过的车站数量，为大于1且不超过100的整数。

- `stations`：车次经过的所有车站名，共`stationNum`项，站名由汉字组成，不超过10个汉字。

  格式为：`station_1 station_2 station_3 ... station_stationNum`

- `seatNum`：该车次的座位数，为不超过100000的整数。

- `prices`：每两站之间的票价，共`(stationNum - 1)`项，第`i`项表示`station_i`到`station_(i+1)`的票价，是一个不超过100000的整数。

  格式为：`price_1 price_2 price_3 ... price_(stationNum - 1)`
  
- `startTime`：列车每日的发车时间，时间格式为`hr:mi`。如：`23:51`。
  
- `travelTimes`：每两站之间行车所用的时间（单位为分钟），共`(stationNum - 1)`项，每一项是一个不超过10000的整数。

- `stopoverTimes`：除始发站和终点站之外，列车在每一站停留的时间（单位为分钟），共`(stationNum - 2)`项，每一项是一个不超过10000的整数。
  
- `saleDate`：车次的售卖时间区间（闭区间），在时间区间内，每天都有相同的此车次。共2项，每一项均为2021年6月至8月的某一天，日期格式为`mm-dd`。如：`06-07`。
  
- `type`：列车类型，为一个大写字母。

#### 指令

每一条指令有一个“常用度”参数，为其在实际情况中的常用程度，也是它在压力测试中的大致出现比例，分为以下几个等级，你可以将其作为设计算法的依据。

- SF：Super frequently used. (~1000000)
- F：Frequently used. (~100000)
- N：Normally used. (~10000)
- R：Rarely used. (~100)

参数的格式为`-<key> <argument>`，其中`<key>`是一个小写字母。为简化描述，在参数列表中省略`<argument>`并在说明中以`-<key>`代指其对应的参数。若不做特殊说明，每个参数都是一个不含空格的字符串。

参数列表的顺序不限，可选参数可以省略，在说明中有可选参数的初始值（如果不为空的话）。

指令的返回值为命令行交互中输出在标准输出流中的执行结果。

- [N] `add_user`

  - 参数列表

    `-c -u -p -n -m -g`

  - 说明

    当前用户`<cur_username>`为`-c`，创建一个`<username>`为`-u`，`<password>`为`-p`，`<name>`为`-n`，`<mailAddr>`为`-m`，`<privilege>`为`-g`的用户。

    权限要求：`-c`已登录，且新用户的权限低于`-c`的权限。

    特别地，创建第一个用户时，新用户权限为`10`，不受上述权限规则约束。具体来讲，当创建第一个用户时，忽略`-c`和`-g`参数，并认为新用户优先级为`10`。

  - 返回值

    注册成功：`0`

    注册失败：`-1`

- [F] `login`

  - 参数列表

    `-u -p`

  - 说明

    以`-u`为`<username>`，`-p`为`<password>`登录。若登录成功，当前用户列表增加此用户。第一次进入系统时，当前用户为空。

  - 返回值

    登录成功：`0`

    登录失败：`-1`
  
- [F] `logout`

  - 参数列表

    `-u`

  - 说明

    `<username>`为`-u`的用户退出登录。若退出成功，将此用户移出当前用户列表。

  - 返回值

    登出成功：`0`

    登出失败：`-1`

- [SF] `query_profile`

  - 参数列表

    `-c -u`

  - 说明

    `<username>`为`-c`的用户查询`<username>`为`-u`的用户信息。

    权限要求：`-c`已登录，且「`-c`的权限大于`-u`的权限，或是`-c`和`-u`相同」。

  - 返回值

    查询成功：一行字符串，依次为被查询用户的`<username>`，`<name>`，`<mailAddr>`和`<privilege>`，用一个空格隔开。

    查询失败：`-1`

- [F] `modify_profile`

  - 参数列表

    `-c -u (-p) (-n) (-m) (-g)`

  - 说明

    `<username>`为`-c`的用户修改`<username>`为`-u`的用户信息。修改参数同注册参数，且均可以省略。

    权限要求：`-c`已登录，且「`-c`的权限大于`-u`的权限，或是`-c`和`-u`相同」，且`-g`需低于`-c`的权限。

  - 返回值

    修改成功：返回被修改用户的信息，格式同`query_profile`。
  
    修改失败：`-1`



- [N] `add_train`
  - 参数列表
  
    `-i -n -m -s -p -x -t -o -d -y`
  
  - 说明
  
    添加`<trainID>`为`-i`，`<stationNum>`为`-n`，`<seatNum>`为`-m`，`<stations>`为`-s`，`<prices>`为`-p`，`<startTime>`为`-x`，`<travelTimes>`为`-t`，`<stopoverTimes>`为`-o`，`<saleDate>`为`-d`，`<type>`为`-y`的车次。
    
    由于`-s`、`-p`、`-t`、`-o`和`-d`由多个值组成，输入时两个值之间以`|`隔开（仍是一个不含空格的字符串）。
    
  - 返回值
  
    添加成功：`0`
  
    添加失败：`-1`
    
  - 举例：
  
    `> add_train -i HAPPY_TRAIN -n 3 -m 1000 -s 上院|中院|下院 -p 115|514 -x 19:19 -t 600|600 -o 5 -d 06-01|08-17 -y G`
    
    `0`
  
- [N] `release_train`

  - 参数列表

    `-i`

  - 说明

    将`<trainID>`为`-i`的车次发布。发布前的车次可被删除，不可发售车票；发售后的车次不可被删除，可发售车票。

  - 返回值

    发布成功：`0`

    发布失败：`-1`

  - 举例：

    `> release_train -i HAPPY_TRAIN `

    `0` 

- [N] `query_train`

  - 参数列表

    `-i -d`

  - 说明

    查询在日期`-d`发车的，`<trainID>`为`-i`的车次的情况，`-d`的格式为`mm-dd`。

  - 返回值

    查询成功：输出共`(<stationNum> + 1)`行。

    ​	第一行为`<trainID> <type>`

    ​	接下来`<stationNum>`行，第`i`行为`<stations[i]> <ARRIVING_TIME> -> <LEAVING_TIME> <PRICE> <SEAT>`，其中`<ARRIVING_TIME>`和`<LEAVING_TIME>`为列车到达本站和离开本站的绝对时间，格式为`mm-dd hr:mi`。`<PRICE>`为从始发站乘坐至该站的累计票价，`<SEAT>`为从该站到下一站的剩余票数。对于始发站的到达时间和终点站的出发时间，所有数字均用`x`代替；终点站的剩余票数用`x`代替。
    
    查询失败：`-1`
    
  - 举例

    （上接添加列车的例子）

    `> query_train -d 07-01 -i HAPPY_TRAIN`
    
    `HAPPY_TRAIN G`
    
    `上院 xx-xx xx:xx -> 07-01 19:19 0 1000`
    
    `中院 07-02 05:19 -> 07-02 05:24 114 1000`
    
    `下院 07-02 15:24 -> xx-xx xx:xx 628 x`

- [N] `delete_train`

  - 参数列表

    `-i`

  - 说明

    删除`<trainID>`为`-i`的车次。

  - 返回值

    删除成功：`0`

    删除失败：`-1`



- [SF] `query_ticket`

  - 参数列表

    `-s -t -d (-p time)`

  - 说明

    查询日期为`-d`时从`-s`出发，并到达`-t`的车票。请注意：这里的日期是列车从`-s`出发的日期，不是从列车始发站出发的日期。
    
    `-p`的值为`time`和`cost`中的一个，若为`time`表示输出按照该车次所需时间从小到大排序，否则按照票价从低到高排序。

  - 返回值

    查询成功： 第一行输出一个整数，表示符合要求的车次数量。

    接下来每一行输出一个符合要求的车次，按要求排序。格式为`<trainID> <FROM> <LEAVING_TIME> -> <TO> <ARRIVING_TIME> <PRICE> <SEAT>`，其中出发时间、到达时间格式同`query_train`，`<FROM>`和`<TO>`为出发站和到达站，`<PRICE>`为累计价格，`<SEAT>`为最多能购买的票数。
    
    查询失败：`-1`

  - 样例

    （上接查询列车的例子）

    `> query_ticket -s 中院 -t 下院 -d 08-17`
    
    `1`
    
    `HAPPY_TRAIN 中院 08-17 05:24 -> 下院 08-17 15:24 514 1000`

- [N] `query_transfer`

  参数列表及其意义同`query_ticket`。

  - 说明

    在恰好换乘一次的情况下查询符合条件的车次，仅输出最优解。

  - 返回值

  	查询失败（没有符合要求的车次）：`0`
  	
  	查询成功：输出2行，换乘中搭乘的两个车次，格式同`query_ticket`。

- [SF] `buy_ticket`
  
  - 参数列表
    `-u -i -d -n -f -t (-q false)`
    
  - 说明
    
    `<username>`为`-u`的用户购买：`<trainID>`为`-i`，日期为`-d`，从站`-f`到站`-t`的车票`-n`张。
    
    `-q`可选`false`或`true`，若为`true`，表明在**余票不足**的情况下愿意接受候补购票，当有余票时**立即**视为此用户购买了车票。
    
    权限要求：`-u`已登录。
    
  - 返回值
  
    购买成功：一个整数，表示订单总价。
    
    加入候补：`queue`
    
    购票失败：`-1`
    
  - 样例
  
    （上接查询车票的例子，假设用户均已登录）
  
    `> buy_ticket -u Texas -i HAPPY_TRAIN -d 08-17 -n 800 -f 中院 -t 下院`
  
    `257000`
  
    `> buy_ticket -u Lappland -i HAPPY_TRAIN -d 08-16 -n 500 -f 上院 -t 下院 -q true`
  
    `queue`
  
- [F] `query_order`

  - 参数列表

    `-u`

  - 说明

    查询`<username>`为`-u`的用户的所有订单信息，按照交易时间顺序从新到旧排序（候补订单即使补票成功，交易时间也以下单时刻为准）。

    权限要求：`-u`已登录。

  - 返回值

    查询成功：第一行输出一个整数，表示订单数量。

    接下来每一行表示一个订单，格式为`[<STATUS>] <trainID> <FROM> <LEAVING_TIME> -> <TO> <ARRIVING_TIME> <PRICE> <NUM>`，其中`<NUM>`为购票数量，`<IS_PENDING>`表示该订单的状态，可能的值为：`success`（购票已成功）、`pending`（位于候补购票队列中）和`refunded`（已经退票）。
    
    查询失败：`-1`
    
  - 样例

    `> query_order -u Lappland`

    `1`

    `[pending] HAPPY_TRAIN 上院 08-17 05:24 -> 下院 08-17 15:24 628 500`

- [N] `refund_ticket`

  - 参数列表

    `-u (-n 1)`

  - 说明

    `<username>`为`-u`的用户退订从新到旧（即`query_order`的返回顺序）第`-n`个订单。

    权限要求：`-u`已登录。

  - 返回值

    退票成功：`0`

    退票失败：`-1`

- [R] `clean`

  - 参数列表

    无

  - 说明

    清除所有数据。

  - 返回值

    `0`

- [R] `exit`

  - 参数列表

    无

  - 说明

    退出程序，所有在线用户均下线。

  - 返回值

    `bye`


## Q&A

注：若文档有更新，也会在这里列出对应的修改。

- Q: 有没有隔天的车？

   A: 有的，且售票日期区间所指的为**起点站发车的时刻**所在的日期，查询或购买车票所指的为**上车的站发车的时刻**所在的日期。

- Q: 再读入时需要判断各种信息的**格式**是否合法吗？

   A: 不需要，保证输入格式合法。

- Q: 候补购票队列优先级是先下单的先满足，不能满足就看下一个可不可以满足吗？

   A: 是的。

- Q: 同优先级的用户可以相互修改吗？

   A: 不可以，见`modify_profile`权限说明（`-c`的权限大于`-u`的权限，或是`-c`和`-u`相同）。

- Q: 有49张票要买50张票，这个50张票的订单全部进入候补队列吗？

   A: 是的，订单为购票和候补的最小单位，均只会**完全满足**。

- Q: 退订第n个订单，序号n是已退订单在内的序号吗？

   A: 是的，相当于`query_order`输出的第n个订单。

- Q: `exit`是真的退出程序还是只在线用户下线？

   A: 退出程序。

- Q: 有没有火车多次经过一个站？

   A: 没有。

- Q: 换乘可以是同一辆车换成同一辆车吗？

   A: 不可以。

- Q: `query_transfer`最优解是指`-p`为`time`时时间最优解，`-p`为`cost`时空间最优解吗？

    A: 是的。

- Q: 查票的时候没有余票情况也要显示该车次吗？

    A: 是的。
- Q: 车次在release之前可以查询，在release后可以查询、购票吗？

    A: 车次在release之前可以：查询车次，删除车次（没有查询车票）；在release之后可以进行删除车次外的任何操作。

- Q: 在后端的接口中，允许多个用户登录到系统，那么在前端的网页实现中，是否也要实现多个用户同时登录的功能？还是按照正常的账户惯例，一次只能登录一个用户？

    A: 每一个前端客户端都可以登录一个用户，可以有很多前端同时打开，且它们的登录状态可以共存（这也就是为什么当前用户列表是一个集合而不是某个特定的用户了）。
    对后端来讲，不同前端发送命令的-c或-u不同，因此对于一个前端只需要判断当前登录的是哪【一个】用户，并且把这个信息作为指令的操作者告诉后端就好了。

- Q: 非第一个用户的时候要求`-c`已登录吗？

    A: 需要，新用户的权限应当【低于】当前用户而非【不高于】。

- Q: 车次经过车站数有限制吗？

    A: 有，至少经过两个站（始发站和终点站），最多经过100个站。

- Q: 车次运行时间长度有限制吗？

    A: 有，不超过3天。

- Q: add_train什么情况下会失败吗？

    A: 在train id冲突的时候。

- Q: 添加用户时id冲突会失败吗?

    A: 会。

- Q: 保证所有输入（指所有操作的输入）的日期都为2021年6月至8月的某一天吗？

    A: 保证时间合法且在2021年范围内。

- Q: query_order在-u没有登陆时应该会操作失败吧？

    A: 会。

- Q: 命令失败主要是哪些形式呢？

    A: 主要是添加时键值冲突，输入不合法（指输入的车站不存在等）或是不符合权限要求。

- Q: query_ticket和query_order失败时返回值？

    A: `-1`

- Q: 评测提交方式？

    A: 通过git提交，在仓库根目录提供Makefile或者cmakelist即可。

- Q: modify_profile中，被修改至的权限有限制吗？

    A: 有的，需要低于`-c`的权限。

- Q: buy_ticket的时间是指？

    A: 和查询一致，是指乘车的时间而非列车从始发站发车的时间。

- Q: 对于只有两站的车次，-o参数是什么？

    A: -o参数内容为一个下划线（`-o _`）

- Q: `query_ticket`会失败吗？

    A: 不会。车站不存在不视为查询失败，而是认为没有符合条件的车次，因此返回`0`。

- Q: 查票操作，时间、票价排序的时候相同的怎么办？

    A: 以train ID字典序为第二排序关键字。如果query_transfer有并列结果，尽量选择在train 1（经过始发站的列车）上乘坐时间更少的方案。

- Q: 如果购票数量大于列车最大座位数还可以候补吗？

    A: 不可以。

