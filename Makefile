FLAGS = -fdiagnostics-generate-patch -fdiagnostics-path-format=inline-events  \
-Og -ggdb -std=c++17 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations   \
-Wc++14-compat -Wuseless-cast -Wmissing-declarations -Wcast-align -Wcast-qual \
-Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy  \
-Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security              \
-Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor       \
-Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self       \
-Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo                     \
-Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn       \
-Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override              \
-Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code         \
-Wunused -Wvariadic-macros -Wno-literal-suffix                                \
-Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast            \
-Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation               \
-fstack-protector -fstrict-overflow -flto-odr-type-merging                    \
-fno-omit-frame-pointer -Wstack-usage=8192                                    \
-fsanitize=address,bool,bounds,enum,float-cast-overflow,$\
float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,$\
object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,$\
undefined,unreachable,vla-bound,vptr

all: Differentiator

Differentiator:  InputText.o Stack.o Tree.o Differentiator.o main.o  
		@g++ ./Apps/InputText.o ./Apps/Stack.o ./Apps/Tree.o ./Apps/Differentiator.o ./Apps/main.o -o Differentiator.exe $(FLAGS)

InputText.o:
		@g++ ./source/InputText.cpp -c -o ./Apps/InputText.o $(FLAGS)

Stack.o:
		@g++ ./source/Stack.cpp -c -o ./Apps/Stack.o $(FLAGS)

Tree.o:
		@g++ ./source/Tree.cpp -c -o ./Apps/Tree.o $(FLAGS)

Differentiator.o:
		@g++ Differentiator.cpp -c -o ./Apps/Differentiator.o $(FLAGS)

main.o:
		@g++ main.cpp -c -o ./Apps/main.o $(FLAGS)

clean:
		./Apps rm -rf *.o

run:
		./Differentiator.exe

image:
		dot tree.dot -T png -o tree.png