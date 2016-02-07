# OpenWRTでキャラクタデバイスを動かす

# preparation

1. このリポジトリをopenwrt/packagesに展開
2. ``make menuconfig``で``Kernel module -> Other modules -> kmod hello driver``をチェック
3. ``make``

# execution

```
## moduleを探す
cat /proc/devices | grep hello
## 見つかったmajor versionを指定して特殊ファイルを生成する
mknod /dev/hello c 253 0
## R/Wできるようになる
cat /dev/hello
echo "hoge" > /dev/hello
```

## 参考

http://www.hakodate-ct.ac.jp/~tokai/tokai/research/kmod.html
