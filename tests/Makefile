# Dirty little tests

EXAMPLE_DIR = ../examples
STDF = stdf.bz2

all:

check: ascii html stats

ascii:
	$(EXAMPLE_DIR)/dump_records_to_ascii $(STDF) > ascii.out
	bzcmp ascii.out ascii.good.bz2

html:
	$(EXAMPLE_DIR)/dump_records_to_html $(STDF) html.out
	bzcmp html.out html.good.bz2

stats:
	$(EXAMPLE_DIR)/record_stats $(STDF) > stats.out
	bzcmp stats.out stats.good.bz2

clean:
	rm -f *.out
