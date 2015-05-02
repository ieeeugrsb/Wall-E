# Small Device C Compiler
SDCC is a open-source compiler for small devices like the processor of the
CC2530 node: Intel 8051. Here are some instructions to get a ready version
of the compiler for this device.
[Original reference](https://github.com/contiki-os/contiki/wiki/8051-Requirements)

1. Get the source, the last version tested is 9092.
```bash
svn co -r 9092 svn://svn.code.sf.net/p/sdcc/code/trunk/sdcc
```
2. Go into the directory
```bash
cd sdcc
```
3. Edit the compiler to adjust to our CC2530 device.
  1. Edit *device/lib/incl.mk* to select *model-huge* and *model-large* libraries. Find the `MODELS` variable and change to
  ```makefile
  MODELS = large huge
  ```
  2. Edit *device/lib/Makefile.in*. Replace `small` with `model` in the following line:
  ```makefile
  TARGETS += models small-mcs51-stack-auto
  ```
4. Run configure disabling everything except what we want. The installation directory will be */opt/sdcc*, you can change it in the `--prefix` argument.
```bash
./configure --disable-gbz80-port --disable-z80-port --disable-ds390-port --disable-ds400-port --disable-pic14-port --disable-pic16-port --disable-hc08-port --disable-r2k-port --disable-z180-port --disable-sdcdb --disable-ucsim --prefix=/opt/sdcc
```
5. Compile and install it
```bash
make
sudo make install
```
6. Finally, set an environment
```bash
source sdcc-env
```
