# Level C

Projeto da disciplina **Programação Imperativa Formativa (PIF)** para criar um remake 2D do jogo "Level Devil", escrito inteiramente em C e utilizando a `cli-lib` para renderização em terminal (ou janela gráfica).

**Equipe:**
* **João**
* **Rafael Coutinho Lima**
* **Guilherme**

---

## 1. Dependências

Para compilar e rodar este projeto, você precisará de:
* `make`
* Um compilador C (como `gcc` ou `clang`)
* A biblioteca `cli-lib` (incluída como submódulo Git)

## 2. Como Compilar e Rodar

### Clonando o Repositório

Como o projeto usa a `cli-lib` como um [submódulo Git](https://git-scm.com/book/en/v2/Git-Tools-Submodules), você **precisa** usar o comando `--recurse-submodules` ao clonar:

```bash
git clone --recurse-submodules [https://github.com/SEU_USUARIO/SEU_REPOSITORIO.git](https://github.com/SEU_USUARIO/SEU_REPOSITORIO.git)
cd Level_C
