libgame: .
	$(MAKE) -C $@/src clean
	$(MAKE) -C $@/src

liblua: .
	$(MAKE) -C $@/src clean
	$(MAKE) -C $@/src

libpoly: .
	$(MAKE) -C $@/src clean
	$(MAKE) -C $@/src

libsql: .
	$(MAKE) -C $@/src clean
	$(MAKE) -C $@/src

libthecore: .
	$(MAKE) -C $@/src clean
	$(MAKE) -C $@/src

game: .
	$(MAKE) -C $@/src clean
	$(MAKE) -C $@/src

db: .
	$(MAKE) -C $@/src clean
	$(MAKE) -C $@/src

libs: 
	$(MAKE) -C libgame/src clean
	$(MAKE) -C libgame/src

	$(MAKE) -C liblua/src clean
	$(MAKE) -C liblua/src

	$(MAKE) -C libpoly/src clean
	$(MAKE) -C libpoly/src

	$(MAKE) -C libsql/src clean
	$(MAKE) -C libsql/src

	$(MAKE) -C libthecore/src clean
	$(MAKE) -C libthecore/src

all: 
	$(MAKE) -C libgame/src clean
	$(MAKE) -C libgame/src

	$(MAKE) -C liblua/src clean
	$(MAKE) -C liblua/src

	$(MAKE) -C libpoly/src clean
	$(MAKE) -C libpoly/src

	$(MAKE) -C libsql/src clean
	$(MAKE) -C libsql/src

	$(MAKE) -C libthecore/src clean
	$(MAKE) -C libthecore/src

	$(MAKE) -C game/src clean
	$(MAKE) -C game/src

	$(MAKE) -C db/src clean
	$(MAKE) -C db/src

clean:
	$(MAKE) -C libgame/src clean
	$(MAKE) -C liblua/src clean
	$(MAKE) -C libpoly/src clean
	$(MAKE) -C libsql/src clean
	$(MAKE) -C libthecore/src clean
	$(MAKE) -C game/src clean
	$(MAKE) -C db/src clean
