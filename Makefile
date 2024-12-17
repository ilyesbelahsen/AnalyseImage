# Définir le compilateur
CXX = g++
CXXFLAGS = -Wall -g $(shell pkg-config --cflags opencv4)
LDFLAGS = $(shell pkg-config --libs opencv4)

# Nom de l'exécutable
TARGET = logo

# Fichiers sources
SRCS = logo.cpp

# Règle par défaut
all: $(TARGET)

# Règle pour compiler l'exécutable
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS) $(LDFLAGS)

# Nettoyage des fichiers compilés
clean:
	rm -f $(TARGET)
