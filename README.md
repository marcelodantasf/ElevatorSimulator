# Instruções:
Faça um programa que defina a rota  a ser percorrida por n elevadores, para atender a demanda de m pessoas. Para isto, um algoritmo que defina a rota a ser percorrida por cada um dos elevadores para atender a demanda das pessoas deve ser desenvolvido, bem como deve ser implementado um código que abstraia o funcionamento de um grupo de elevadores (já discutido em sala).

### Considerações gerais:

As pessoas podem estar dentro de um dos elevadores, bem como podem estar fora do elevador em algum andar com o intuito de ir para outro andar de destino.
As pessoas informam o andar de destino assim que entram no elevador. 
Uma pessoa que está fora do elevador aperta o botão de subir ou descer para "chamar"o elevador. 
Os n elevadores tem o mesmo número de andares e dão acesso a torre de interesse, ou seja, qualquer um dos elevadores atende (permite acesso) ao local (apartamento, loja, etc) de destino de uma pessoa.

**Na simulação/modelagem de um elevador, considere que:**

um elevador se locomove com velocidade de 1 andar/segundo. Por exemplo, em 5 segundos um elevador qualquer se locomove do 5o ao 10o andar; ou em 3 segundos  um elevador se locomove do 3o ao 6o andar.
o tempo que as pessoas levam para entrar ou sair é muito pequeno, ou seja, pode considerar que as pessoas entram ou saem de forma instantânea.
o elevador verifica de segundo em segundo se precisa criar/alterar a rota, inclusive para verificar se precisa parar em algum andar para que pessoas saiam ou entrem em algum elevador.
um string com informação do estudo de caso a ser executado pelo algoritmo vai ser lido para indicar/simular o estado atual dos elevadores e quando (em períodos discretos de 1 segundo) pessoas vão solicitar o serviço de um elevador. Exemplo	

**São parâmetros a serem considerados:**

número n de elevadores, em que n pode variar de 1 até 6.
número a de andares, em que n pode variar de 5 até 25.
	
Estudos de casos serão usados para avaliar o desempenho do programa/algoritmo de geração de rotas. Para isto, a métrica a ser utilizada é o número total de andares percorrido para atender a demanda dos estudos de caso. Ou seja, quanto menos andares forem percorridos mais eficiente é o programa/algoritmo.





### Estudo de caso 1:

E1 - elevador 1
E2 - elevador 2
—- | | —---------
En - elevador n

P01 - pessoa 1
P02 - pessoa 2
—- | | —---------
Pm- pessoa m


AM_25 
E1_04_S_6,9,8  
E2_11_D_5,8,9,3,2,T 
E3_20_D_5,8,T,9 

T5_P01_S_4_6 
T8_P01_D_6_4 
T9_P02_S_5_10
T10_P03_S_3_9 
T10_P04_D_6_T 
T10_P05_S_8_15 
T15_P06_D_9_2 
T15_P07_S_2_13 
T18_P08_D_8_T 
T21_P01_D_16_3 
T21_P10_S_T_13 
T21_P11_S_T_12 
T23_P12_S_T_15 
T28_P13_S_2_13

AM_25  signifca que o andar máximo é o 25o.

E1_04_S_6,9,8  significa que o elevador está no 4o andar, subindo e que as pessoas no elevador tinham apertado os andares 6, 9 e 8.

T5_P01_S_4_6 significa que a pessoa 1 no tempo 5 segundos estava no 4o andar e quando ela entrar no elevador ela irá apertar o botão para ir ao 6o andar.