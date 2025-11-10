#ifndef ASSETS_H
#define ASSETS_H

#include <stdbool.h>

// Inicializa/verifica assets e estrutura minima de pastas.
// Retorna false se algo critico estiver faltando.
bool assets_init(void);

// Descarta recursos carregados (texturas, fontes, sons...), se houver.
void assets_unload(void);

#endif // ASSETS_H
