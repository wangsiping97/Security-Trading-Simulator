# Security Trading Simulator
## 测试版本
- 目前仅支持 `OS X` 系统。
- 进入可执行文件所在目录(/bin)，然后在终端下输入`./main`即可。
- 结束程序时（模拟每日交易结束时），需退回最初状态，在 I/S 问题后输入 `X`，将会更新数据并退出。
- 注意！**千万不要手动关闭终端**，未成交的投标将无法回到账户，您的收益率将受损严重。
- 目前股票池中有且仅有一只股票，代码为 `000001`。可根据程序提示信息进行读写操作。
- 支持手动添加股票，具体方法为：
  - 进入 `/data/stock` 目录，新建二进制文件，名称为股票代码（如 `000002`）。
  - 数据结构为：(string) password\n (double)price\n (int)floats_available\n (string)industry\n (int)floats\n (double)roa\n (double)roe\n
- **5月11日更新：支持多用户在多终端内同时启动多个程序进行操作**
- 欢迎测试以及汇报 bug : )