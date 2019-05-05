# Security Trading Simulator
## 测试版本
- 目前仅支持 `OS X` 系统。
- 进入可执行文件所在目录(/bin)，然后在终端下输入`./main`即可。
- 目前股票池中有且仅有一只股票，代码为 `000001`。可根据程序提示信息进行读写操作。
- 支持手动添加股票，具体方法为：
- - 进入 `/data/stock` 目录，新建二进制文件，名称为股票代码（如 `000002`）。
- - 数据结构为：(string) password\n (double)price\n (int)floats_available\n (string)industry\n (int)floats\n (double)roa\n (double)roe\n
- 欢迎测试以及汇报 bug : )