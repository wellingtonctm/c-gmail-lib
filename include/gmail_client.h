#ifndef GMAIL_CLIENT_H
#define GMAIL_CLIENT_H

/**
 * Envia uma requisição GET para a API do Gmail.
 *
 * Esta função faz uma requisição GET para um endpoint da API do Gmail, 
 * como "messages" ou "threads", e retorna a resposta como uma string JSON.
 *
 * A função usa o token de acesso OAuth2 para autenticação, que deve ser 
 * obtido previamente e gerido pelo cliente.
 *
 * @param endpoint O endpoint da API do Gmail a ser acessado (e.g., "messages", "threads").
 * @return Resposta da API como uma string JSON. A memória deve ser liberada pelo chamador. Retorna NULL em caso de erro.
 */
char *gmail_request(const char *endpoint);

/**
 * Envia uma requisição POST para a API do Gmail.
 *
 * Esta função realiza uma requisição POST para o endpoint da API do Gmail.
 * Ela pode ser usada para ações como enviar um e-mail, modificar rótulos, 
 * ou outros tipos de operações que requerem envio de dados.
 *
 * A função usa o token de acesso OAuth2 para autenticação, que deve ser 
 * obtido previamente e gerido pelo cliente.
 *
 * @param endpoint O endpoint da API do Gmail a ser acessado (e.g., "messages/send").
 * @param payload O payload em formato JSON que será enviado na requisição POST.
 * @return Resposta da API como uma string JSON. A memória deve ser liberada pelo chamador. Retorna NULL em caso de erro.
 */
char *gmail_post_request(const char *endpoint, const char *payload);

#endif // GMAIL_CLIENT_H