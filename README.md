

# Edge 客户端

```
./edge 使用说明
欢迎使用 n2n v.2.3.0 for Linux
编译时间: Xxx xx xxxx xx:xx:xx
AES 加密由 OpenSSL 1.1.1x-dev 提供

edge -d <tun 设备> -a [static:|dhcp:]<tun IP 地址>/<前缀长度> -c <社区> [-k <加密密钥> | -K <密钥文件>] [-u <用户ID> -g <组ID>] [-f] [-m <MAC 地址>] -l <超级节点 host:port> [-4|-6] [-p <本地端口>] [-M <MTU>] [-r|-R <路由>] [-E] [-v] [-t <管理端口>] [-b] [-h]

-d <tun 设备>            | tun 设备名称
-a <模式:IPv4/前缀长度>  | 设置接口 IPv4 地址。DHCP 使用 '-r -a dhcp:0.0.0.0/0'
-A <IPv6>/<前缀长度>      | 设置接口 IPv6 地址，仅当 IPv4 为 static 时支持
-c <社区>                | Edge 所属 n2n 社区名称
-k <加密密钥>             | 加密密钥 (ASCII)，也可通过环境变量 N2N_KEY 设置。不能与 -K 同时使用
-K <密钥文件>             | 指定要加载的密钥文件。不能与 -k 同时使用
-l <超级节点 host:port>   | 超级节点 IP:端口
[-4|-6]                   | 解析超级节点 DNS 为 IPv4 或 IPv6（默认未指定）
-b                         | 定期解析超级节点 IP（当超级节点运行在动态 IP 上时）
-p <本地端口>              | 固定本地 UDP 端口
-u <用户ID>                | 降低权限后使用的用户 ID（数字）
-g <组ID>                  | 降低权限后使用的组 ID（数字）
-f                         | 不以守护进程方式运行，而是在前台运行
-m <MAC 地址>              | 固定 TAP 接口 MAC 地址（否则随机），例: -m 01:02:03:04:05:06
-M <MTU>                   | 设置 n2n Edge 接口 MTU（默认 1400）
-r                         | 允许通过 n2n 社区转发数据包
-R <目标>/<长度>,<网关>     | 允许转发数据包并添加路由，IPv4/6 自动检测
-E                         | 接受组播 MAC 地址（默认丢弃）
-v                         | 增加详细输出，可重复使用
-t <管理端口>               | 管理套接字 (UDP 端口或绝对路径)，默认 5644

环境变量:
  N2N_KEY                  | 加密密钥 (ASCII)。不能与 -K 或 -k 同时使用

```

你需要在每一台希望与同一个社区连接的主机上启动一个 edge 节点。

例如
```
./edge -f -d n2n0 -c mynetwork -u 99 -g 99 -k encryptme -m 00:FF:12:34:56:78 -a 192.168.254.1 -s 255.255.255.0 -l a.b.c.d:xyw
```
或
```
N2N_KEY=encryptme ./edge -f -d n2n0 -c mynetwork -u 99 -g 99 -m 00:FF:12:34:56:78 -a 192.168.254.1 -s 255.255.255.0 -l a.b.c.d:xyw
```

在你确认其运行正常后，你可以去掉 `-f` 选项，让 edge 脱离前台并作为守护进程运行。

注意：`-u`、`-g` 和 `-f` 选项在 Windows 上不可用。

在 Windows 上运行 edge 时需要兼容的 TAP 网络接口，该驱动包含在
[OpenVPN installer](https://openvpn.net/index.php/open-source/downloads.html) 中。如果系统存在多个 TAP 适配器，可以使用 `-d` 参数指定适配器的“友好名称”
（即控制面板网络适配器列表中显示的名称）。如果名称包含空格，请务必使用引号。

# Supernode 服务端

```
./supernode 使用说明
-l <lport>      设置 UDP 主监听端口为 <lport>
-4              使用 IPv4 网络（默认）
-6              使用 IPv6 网络
-t              使用 UNIX 管理套接字
-f              前台运行
-v              增加输出详细程度，可多次使用
-h              显示此帮助信息

```

你只需要启动一次 supernode，它不需要任何权限。

1. `./supernode -l 1234 -v -f`

# IPv6 支持

此版本的 edge 和 supernode 支持通过 IPv6 进行传输。

通过指定 `-6` 或 `-4` 与 `-6` 选项来启动 supernode 的 IPv6 或 IPv4+IPv6 模式。

```
./supernode -6 -f
```
或
```
./supernode -4 -6 -f
```


默认设置为仅 IPv4 模式启动 supernode。

要在 edge 中使用 IPv6，只需要为 supernode 指定 IPv6 地址。

```
./edge -f -v [ 其他参数 ] -l [2001:aa00:bb00::1]:1234
```

如果需要 DNS 名称解析 IPv6，则使用 `-6` 参数来强制解析 IPv6 地址。

```
./edge -f -v [ 其他参数 ] -6 -l example.com:1234
```


# IPv6 支持 (内部)

n2n 支持在 n2n 隧道内传输 IPv6 数据包。

```
./edge -f -v [ 其他参数 ] -a 192.168.254.1 -s 255.255.255.0 -A fdf0:aa01:bb02::1/64
```

# 将 Edge 作为系统服务运行

## 使用 net_admin 功能（LINUX）

此版本的 Edge 具有功能感知能力，如果在其允许的权限集中找到 `NET_ADMIN` 该功能，则会使用该功能。

要以 root 用户身份使用此功能集，需要具备以下管理员权限：:

```
setcap cap_net_admin+p ./edge
```

Edge 现在可以由任何具有执行权限的用户运行


## 以守护进程方式运行（UNIX）

除非 `-f` 作为命令行选项指定，否则 edge 会 `daemon(3)` 在设置成功后调用。这会导致进程 fork 出一个子进程，该子进程会关闭自身 `stdin` ， `stdout` 然后 `stderr` 将自身设置为进程组领导者。

完成此操作后，edge 命令会立即返回，您将只会在进程列表中看到 edge 进程，例如在进程列表中或进程列表 `ps` 中 `top` 。

如果 edge 命令返回 0，则表示守护进程已成功启动。如果返回非零值，则表示 edge 由于某种原因启动失败。当 edge 作为守护进程运行时，所有日志都会发送到系统日志 (syslog)`daemon.info`服务。


## 放弃 Root 权限和 SUID-Root 可执行文件（UNIX）

边缘节点使用超级用户权限创建 TAP 网络接口设备。创建完成后，不再需要 root 权限，但如果攻击者能够以某种方式控制正在运行的边缘进程，则可能构成安全隐患。

如果您指定了 `--privilegeduser` 和 `--privilegeduser` `-u <uid>` 选项，边缘节点将切换到非特权用户 `-g <gid>`。这些是数字 ID。请参阅相关文档`/etc/passwd`。

您可以选择安装 edge SUID-root 来实现此目的:

1. Become root
2. `chown root:root edge`
3. `chmod +s edge`

现在任何用户都可以运行 Edge 浏览器。您可能并不需要这样做，但如果您的主机只有一个登录用户，那么这样做可能既方便又安全。

## Systemd 服务（LINUX）

`systemd/` 文件夹中提供了 systemd 服务文件的示例。edge 服务使用 `AmbientCapabilities` 设置 `net_admin` 权限，以便以动态创建的用户身份运行 edge 服务。

Systemd 需要 Linux 内核 4.3 或更高版本才能使用此功能。


## SCM 服务 (WINDOWS)

edge 和 supernode 可以安装为 Windows SCM 服务，请参阅 [`win32/install.ps1`](win32/install.ps1) 存储库中的脚本。

当以服务形式运行时，edge 和 supernode 不会连接到控制台，消息会记录在 Windows 事件日志中。

命令行参数存储在 `HKLM:\SOFTWARE\n2n\edge\Arguments` 和 `HKLM:\SOFTWARE\n2n\supernode\Arguments`中。

可以使用 `<your-command> ` 或管理员权限的 PowerShell 控制台 `HKLM:\SOFTWARE\n2n\supernode\Arguments` 修改它们。

这两个条目都是`MultiStrings`类型，因此可以正确支持包含空格的参数，如果不需要支持空格，也可以使用 *String* 类型。
