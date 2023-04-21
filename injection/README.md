# Specs
Flavor: [Artix-plasma-openrc-x86_64](https://artixlinux.org/download.php)

Kernel: 6.2.6-artix1-1

CPU: Intel i7-8565U

# Dependencies 
Packages used: nasm-2.16.01-1

To update upgrade packages use `sudo pacman -Syu`

To install nasm use `sudo pacman -S nasm`

# Instructions 

- To assemble and link use the `./assemble.sh` script.
- `inject` will appear in your directory
- copy `hello` executable from `hello_dir` directory to your directory
- run `inject`
- `inject` will be rename to `hello_rename` and `hello` will be renamed to `hello_virus`
- for clarity delete `hello_rename`
- copy another `hello` executable and run `./hello_virus`
- `hello_virus` will produce output `hello world` and rename itself `hello_rename`. `hello` will be named `hello_virus`.

1. `./assemble.sh`
2. `cp ./hello_dir/hello ./`
3. `./inject`
4. `ls`
```
hello_rename	hello_virus...
```
5. `rm hello_rename`
6. `cp ./hello_dir/hello ./`
7. `./hello_virus`
```
hello world!
```
8. `ls`
```
hello_rename	hello_virus...
```
