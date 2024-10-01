Edge Computing & IoT: Sistema de Controle de Jogo com ESP32 e MQTT

RICARDO FERNANDES DE AQUINO (RM554597)
KAUÃ SOARES GUIMARÃES (RM559044)
DAYANA TICONA QUISPE (RM558023)
FABRINI SOARES(RM557813)
RICKELMYN DE SOUZA RUESCAS (RM556055)


Introdução
Este projeto consiste no desenvolvimento de um sistema IoT utilizando Edge
Computing para capturar dados de um joystick e um sensor ultrassônico
conectados a um ESP32. Os dados capturados são enviados via MQTT para um
Node-RED, onde são processados e exibidos em um dashboard e processados em
um front-end de controle de jogo.
O principal objetivo do projeto é demonstrar como dispositivos conectados
podem coletar, processar e transmitir dados em tempo real, utilizando uma
arquitetura IoT. Isso envolve a integração de dispositivos IoT, um broker MQTT e um
sistema de visualização de dados.


Arquitetura Proposta
A arquitetura do sistema IoT é composta pelos seguintes componentes:
1. ESP32: Dispositivo IoT principal, responsável pela leitura dos dados do
joystick e do sensor ultrassônico.
2. Sensor Ultrassônico: Utilizado para medir a distância e simular a força de
sinal Bluetooth.
3. Joystick: Usado para capturar movimentos e botões de ação do jogo.
4. Broker MQTT (Mosquitto): Responsável por receber e distribuir mensagens
publicadas pelo ESP32.
5. Node-RED: Ferramenta para processar os dados recebidos do MQTT e
exibir em um dashboard gráfico.
6. Front-End Web: Interface do usuário desenvolvida para receber e
processar os dados do ESP32 via MQTT e controlar o jogo.


Fluxo de Dados:
1. O ESP32 capta os dados do joystick (movimento e botões) e do sensor
ultrassônico (distância).
2. O ESP32 publica essas informações via MQTT:
o Tópico dash/game/movimento: Para os dados de movimento do
joystick.
o Tópico dash/game/sinal: Para os dados de força do sinal (simulado
pelo sensor ultrassônico).
3. O broker MQTT recebe as publicações e encaminha os dados para o NodeRED e o front-end.
4. O Node-RED exibe os dados no dashboard em tempo real, e o front-end
usa as informações para controlar o jogo.

Diagrama da Arquitetura(imagem anexada)

Recursos Necessários
Hardware:
• ESP32: Placa de desenvolvimento com Wi-Fi e Bluetooth.
• Joystick: Para controle de movimentos e botões de ação.
• Sensor Ultrassônico: Para medir a distância e simular a força do sinal
Bluetooth.
• LCD (16x2 I2C): Exibição de informações diretamente no dispositivo IoT.

Software:
• Arduino IDE: Para programar o ESP32.
• Broker MQTT (Mosquitto): Para gerenciamento das mensagens entre o
ESP32 e o Node-RED.
• Node-RED: Para processar e exibir os dados no dashboard.
• Broker Público MQTT: test.mosquitto.org (pode-se usar um broker local).
• Front-End Web (MQTT.js): Interface de controle para o jogo.


Instruções de Instalação
1. Configuração do ESP32
    1. Baixe e instale a Arduino IDE.
    2. Adicione o suporte à ESP32 nas placas (via Gerenciador de Placas).
    3. Conecte o ESP32 ao computador via cabo USB.
    4. Carregue o código.
2. Configuração do Broker MQTT
    1. Instale o Mosquitto localmente (opcional) ou utilize um broker público
    como test.mosquitto.org.
    2. Configure o broker para receber as mensagens do ESP32.
3. Configuração do Node-RED
    1. Instale o Node-RED (https://nodered.org/).
    2. Crie os fluxos para receber os dados dos tópicos MQTT e exibi-los no
    dashboard:
    -- dash/game/movimento: Para dados do movimento.
    -- dash/game/sinal: Para dados de força de sinal.
    3. Configure o dashboard para exibir as informações em tempo real.


Implementação do Front-End
O front-end será responsável por receber as mensagens publicadas pelo ESP32
via MQTT e controlar o jogo. Utilize o MQTT.js para realizar a comunicação.

1. Instalação do MQTT.js
No diretório do projeto front-end, execute:
npm install mqtt --save

2. Exemplo de Código para Front-End:
const mqtt = require('mqtt');
const client = mqtt.connect('mqtt://test.mosquitto.org');
client.on('connect', () => {
 client.subscribe('dash/game/movimento', (err) => {
 if (!err) {
 console.log('Subscribed to movimento topic');
 }
 });
 client.subscribe('dash/game/sinal', (err) => {
 if (!err) {
 console.log('Subscribed to sinal topic');
 }
 });
});
client.on('message', (topic, message) => {
 if (topic === 'dash/game/movimento') {
 console.log(`Movimento recebido: ${message.toString()}`);
 // Ações com base no movimento (esquerda, direita, frear, acelerar)
 }
 if (topic === 'dash/game/sinal') {
 console.log(`Força do sinal recebida: ${message.toString()}`);
 // Ações com base no sinal recebido
 }
});


Dependências
Para o ESP32:
    Bibliotecas para Arduino IDE:
    -- PubSubClient: Para comunicação MQTT.
    -- Wire: Para comunicação I2C com o LCD.
    -- Ultrasonic: Para leitura do sensor ultrassônico.
    -- LiquidCrystal_I2C: Para controle do display LCD via I2C.

Para o Front-End:
    Node.js e MQTT.js para comunicação com o broker MQTT.


Instruções de Uso
1. Conecte o ESP32 e configure a rede Wi-Fi.
2. Execute o Node-RED e visualize o dashboard.
3. Controle o jogo usando o joystick e visualize os dados no display LCD e no
dashboard do Node-RED.
4. Monitore a força do sinal simulada pelo sensor ultrassônico, representando
a proximidade do objeto no display gráfico.


Códigos-Fonte
    Todo o código fonte necessário para o funcionamento do ESP32 está
    incluído no repositório. Ele contém:
    -- Leitura do joystick e sensor ultrassônico.
    -- Publicação dos dados via MQTT.
    -- Exibição das informações no LCD.