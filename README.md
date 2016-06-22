# hush
shell for humans

#### how to run
to build hush, do:
``` shell
cd readline
./configure
make
cd ..
make
```
to run, do
``` shell
bin/hush
```

#### how to use
first, you should do the `help` command

some examples:
``` shell
apropos shell | grep man | less
man echo > some_file
hexdump -C < some_file > file
grep display < file
less file
```

#### features
- pipes
- file input/output operations
- autocomplete paths on <kbd>tab</kbd>
- history
- emacs bindings to work with command
