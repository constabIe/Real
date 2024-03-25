CC = g++

CFLAGS = -std=c++14 -Wall -Wextra -lgtest -lgtest_main -pthread

BGREEN  = \033[1;32m
BYELLOW = \033[1;33m
GREEN   = \033[1;35m
BCYAN   = \033[1;36m
RESET   = \033[0m

googletest_deployment:
	@printf "${BYELLOW}//----------------------------------//\n"
	@printf "// Googletest system deployment ... //\n"
	@printf "//----------------------------------//${RESET}\n"

	@git clone https://github.com/google/googletest.git   
	@make build_googletest

	@printf "${BCYAN}//----------//\n"
	@printf "// Success! //\n"
	@printf "//----------//${RESET}\n"

tests_launch:
	@printf "${BYELLOW}//---------------------//\n"
	@printf "// Tests launching ... //\n"
	@printf "//---------------------//${RESET}\n"
	@make build_tests run_tests

	@printf "${BCYAN}//----------//\n"
	@printf "// Success! //\n"
	@printf "//----------//${RESET}\n"

clean_all:
	@printf "${BYELLOW}//----------------------//\n"
	@printf "// Cleaning process ... //\n"
	@printf "//----------------------//${RESET}\n"

	@make clean_tests clean_googletest

	@printf "${BCYAN}//----------//\n"
	@printf "// Success! //\n"
	@printf "//----------//${RESET}\n"

.PHONY: tests_launch clean_all

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

build_googletest:
	@mkdir $@
	@cd $@ && cmake ../googletest
	@cd $@ && make && make install

clean_googletest:
	@rm -rd googletest
	@rm -rd build_googletest

# tests_launcher:
build_tests:
	@${CC} ./src/Real.cpp ./tests/test.cpp ${CFLAGS} -o ./tests/test

run_tests:
	@./tests/test

clean_tests:
	@rm -rd ./tests/test





