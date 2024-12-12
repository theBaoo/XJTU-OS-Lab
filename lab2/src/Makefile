.PHONY: all clean
SUBDIRS := interrupt pipe mem page

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

clean:
	for dir in $(SUBDIRS); do   \
		$(MAKE) -C $$dir clean; \
	done