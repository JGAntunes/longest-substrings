
SUBDIRS =  lib tests

CFLAGS = -g -ansi -Wall

export DEFS
export CFLAGS

all: build

build:
	@list='$(SUBDIRS)'; for p in $$list; do \
	  echo "Building $$p"; \
	  $(MAKE) -C $$p; \
	done

clean:
	@list='$(SUBDIRS)'; for p in $$list; do \
	  echo "Cleaning $$p"; \
	  $(MAKE) clean -C $$p; \
	done

package: clean zip

zip:
ifndef CAMPUS
	@echo "ERROR: Must setup macro 'CAMPUS' correcly."
else
ifndef CURSO
	@echo "ERROR: Must setup macro 'CURSO' correcly."
else
ifndef GRUPO
	@echo "ERROR: Must setup macro 'GRUPO' correcly."
else
	tar -czf project-$(CAMPUS)-$(CURSO)-$(GRUPO)-$(ALUNO1)-$(ALUNO2)-$(ALUNO3).tgz *
endif
endif
endif
