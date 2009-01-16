#!/usr/bin/make -f
SHIP_DIR=../pret_ship

ship: configure
# Make shipping directory
	mkdir ${SHIP_DIR}
	-cp -r . ${SHIP_DIR}
# Start changes
	-mkdir ${SHIP_DIR}/examples
# Copy ALL instructions and data in Janne
	-cp -r tests/tests/pret/dma/janne/janne ${SHIP_DIR}/examples/ 
# Copy Smile example
	-cp -r tests/tests/pret/sys/smile/smile ${SHIP_DIR}/examples/ 
# Copy Synchronization example
	-cp -r tests/tests/pret/dma/sync_example_cases/sync_example_cases ${SHIP_DIR}/examples/
# Copy graphic example
	-cp -r soft/graphic ${SHIP_DIR}/examples/
# Remove .svn and other directories.
	-rm -rf ${SHIP_DIR}/tests/ ${SHIP_DIR}/soft/ ${SHIP_DIR}/adm/ \
    ${SHIP_DIR}/autom4te.cache/ ${SHIP_DIR}/logs/ \
    ${SHIP_DIR}/oldfiles/ ${SHIP_DIR}/aclocal.m4 ${SHIP_DIR}/config.h.in \
    ${SHIP_DIR}/format_code.sh ${SHIP_DIR}/ship.sh \
    ${SHIP_DIR}/bootstrap.sh ${SHIP_DIR}/configure.in
	-find ${SHIP_DIR} -name ".svn" | xargs rm -rf	 
	-find ${SHIP_DIR} -name "*~" | xargs rm -f
	-mv ${SHIP_DIR}/Makefile.dist.am ${SHIP_DIR}/Makefile.am
	-mv ${SHIP_DIR}/configure.dist.in ${SHIP_DIR}/configure.in
    
configure:
	./bootstrap.sh
