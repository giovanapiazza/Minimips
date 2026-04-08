MiniMIPS: Emulador de Processador em C
Este repositório contém a implementação de um MiniMIPS, um emulador simplificado da arquitetura MIPS, desenvolvido como parte da disciplina de Projeto Integrador II. 
O objetivo do projeto foi construir, de forma modular e incremental, um sistema capaz de carregar, decodificar e executar instruções de máquina.

*Sobre o Projeto*
O projeto foi dividido em etapas evolutivas, onde cada fase adicionou uma nova camada de complexidade ao hardware simulado. O emulador foi desenvolvido inteiramente em C, focando na manipulação de memória, registradores e no ciclo de instrução (Fetch, Decode, Execute).

*Etapas de Desenvolvimento*
**Módulo de Memória:** Implementação da estrutura de memória para armazenar dados e instruções.

**Busca (Fetch) e Decodificação:** Lógica para leitura de binários e identificação dos campos de instrução (Opcode, RS, RT, RD, etc.).

**Unidade Lógica e Aritmética (ULA):** Implementação das operações matemáticas e lógicas fundamentais.

**Ciclo de Execução:** Integração completa para rodar programas simples em linguagem de máquina MIPS.

*Arquitetura Implementada*
**O processador simula os componentes básicos de um MIPS real:**

**Registradores:** Banco de registradores de 32 bits.

**PC (Program Counter):** Controle do fluxo de execução.

**Memória:** Simulação de memória RAM para dados e instruções.
