
app := app_demo
$(app)_SOURCES := app_demo.c
$(app)_LINK_LIBS = $(addprefix -l, tr50 ssl crypto pthread dl)

all:
	$(CC) -Wall -c $($(app)_SOURCES)
	$(CC) -Wall $(patsubst %c,%o,$($(app)_SOURCES)) -o $(app) $($(app)_LINK_LIBS)

clean:
	rm -f *.o $(app)
