#ifndef PROGRESS_H
#define PROGRESS_H

// Função para definir qual nível queremos jogar
void progress_set_current_level(int level_id);

// Função para buscar qual nível foi selecionado
int progress_get_current_level(void);

#endif