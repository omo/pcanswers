
all: a168

a%: p%.solver
	./$< < ${subst .solver,, $<}.txt
p%.solver: p%.cpp
	g++ -g -Wall $< -o $@

clean:
	-rm *.solver