#ifndef PROGRESS_H
#define PROGRESS_H
#include <stdbool.h>
#define MAX_LEVELS_SUPPORTED 20

void progress_init(void);
// Função para definir qual nível queremos jogar
void progress_set_current_level(int level_id);
// Função para buscar qual nível foi selecionado
int progress_get_current_level(void);

void progress_complete_current_level(void);
//verifica se foi concluido
bool progress_is_level_completed(int level_id);

#endif