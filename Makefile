VER := v0.0.1
minilsar: minilsar.c
	$(CC) -o minilsar -std=c99 minilsar.c -larchive -DVERSION=\"$(VER)\"
