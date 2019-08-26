#detecta se o sistema é de 32 ou 64 bits
SYS = $(shell uname)
BITS_OPTION = -m64


ifeq ($(SYS),Darwin)
   SYSTEM  = x86-64_osx
else
   SYSTEM  = x86-64_linux
endif


LIBFORMAT = static_pic

####diretorios com as libs do cplex
ifeq ($(SYS),Darwin)
	CPLEXDIR      = /Users/VitorVesello/ILOG/CPLEX_Studio126/cplex
	CONCERTDIR    = /Users/VitorVesello/ILOG/CPLEX_Studio126/concert
else
	CPLEXDIR      = /usr/ilog/cplex
	CONCERTDIR    = /usr/ilog/concert
endif

CPLEXLIBDIR   = $(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CONCERTLIBDIR = $(CONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT)

#### define o compilador
CPPC = g++
#############################

#### opcoes de compilacao e includes
ifeq ($(SYS),Darwin)
	CCOPT = $(BITS_OPTION) -O0 -O -fPIC -fexceptions -DNDEBUG -DIL_STD -stdlib=libstdc++
else
	CCOPT = $(BITS_OPTION) -O3 -g -fPIC -fexceptions -DNDEBUG -DIL_STD -std=c++0x
endif
CONCERTINCDIR = $(CONCERTDIR)/include
CPLEXINCDIR   = $(CPLEXDIR)/include
CCFLAGS = $(CCOPT) -I$(CPLEXINCDIR) -I$(CONCERTINCDIR)
#############################

#### flags do linker
#ORIGINAL: CCLNFLAGS = -L$(CPLEXLIBDIR) -lilocplex -lcplex -L$(CONCERTLIBDIR) -lconcert -lm -lpthread 
ifeq ($(SYS),Darwin)
	CCLNFLAGS = -L$(CPLEXLIBDIR) -lilocplex -lcplex -L$(CONCERTLIBDIR) -lconcert -lm -lpthread -framework CoreFoundation -framework IOKit -stdlib=libstdc++
else
	CCLNFLAGS = -L$(CPLEXLIBDIR) -lilocplex -lcplex -L$(CONCERTLIBDIR) -lconcert -lm -lpthread 
endif
#############################

#### diretorios com os source files e com os objs files
SRCDIR = src
OBJDIR = obj
#############################

#### lista de todos os srcs e todos os objs
SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCS))
#############################


#### regra principal, gera o executavel
rsa: $(OBJS)
	@echo  "\033[31m \nLinking all object files: \033[0m"
	$(CPPC) $(BITS_OPTION) $(OBJS) -o $@ $(CCLNFLAGS)
############################

	@echo  "\033[34m \nMakefile: $(SYS) \033[0m"


#inclui os arquivos de dependencias
#-include $(OBJS:.o=.d)
-include $(OBJS:.o)


#regra para cada arquivo objeto: compila e gera o arquivo de dependencias do arquivo objeto
#cada arquivo objeto depende do .c e dos headers (informacao dos header esta no arquivo de dependencias gerado pelo compiler)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@echo  "\033[31m \nCompiling $<: \033[0m"
	$(CPPC) $(CCFLAGS) -c $< -o $@


# $(CPPC)  -MM $< > $(basename $@).d
# @mv -f $(basename $@).d $(basename $@).d.tmp #proximas tres linhas colocam o diretorio no arquivo de dependencias (g++ nao coloca, surprisingly!)
# @sed -e 's|.*:|$(basename $@).o:|' < $(basename $@).d.tmp > $(basename $@).d
# @rm -f $(basename $@).d.tmp

#delete objetos e arquivos de dependencia
clean:
	@echo "\033[31mcleaning obj directory \033[0m"
	@rm -f $(OBJDIR)/*.o $(OBJDIR)/*.d

rebuild: clean rsa
