#ifndef PROGRESS_H
#define PROGRESS_H
#include <stdbool.h>
#define MAX_LEVELS_SUPPORTED 20

void progress_init(void);
// Função para definir qual nível queremos jogar
void progress_save(void);
//salvar o progresso
void progress_set_current_level(int level_id);
// Função para buscar qual nível foi selecionado
int progress_get_current_level(void);

void progress_complete_current_level(void);
//verifica se foi concluido
bool progress_is_level_completed(int level_id);
// Necessária para o map_screen.c saber até onde printar os cadeados nas bloqueadas
int progress_get_max_unlocked(void);
//para contar as mortes
void progress_add_death(void);

//para retornar o valor das mortes
int progress_get_total_deaths(void);

#endif