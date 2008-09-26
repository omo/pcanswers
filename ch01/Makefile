
all: a165

a%: p%.solver
	./$< < ${subst .solver,, $<}.txt
p%.solver: p%.cpp
	g++ -Wall $< -o $@

clean:
	-rm *.solver