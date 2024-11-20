#ifndef GMAIL_MESSAGES_H
#define GMAIL_MESSAGES_H

/**
 * Listar mensagens da caixa de entrada do Gmail.
 * 
 * @param query Filtro de busca (ex: "is:unread"). Pode ser NULL para listar todas.
 * @param max_results Número máximo de mensagens a listar. Use 0 para o padrão.
 * @return JSON com a lista de mensagens. Deve ser liberado pelo chamador.
 */
char *gmail_list_messages(const char *query, int max_results);

/**
 * Obter detalhes de uma mensagem específica.
 * 
 * @param message_id O ID da mensagem.
 * @return JSON com os detalhes da mensagem. Deve ser liberado pelo chamador.
 */
char *gmail_get_message(const char *message_id);

/**
 * Enviar um e-mail pelo Gmail.
 *
 * @param to Destinatário do e-mail.
 * @param subject Assunto do e-mail.
 * @param body Conteúdo do e-mail.
 * @return 0 em caso de sucesso, -1 em caso de erro.
 */
int gmail_send_email(const char *to, const char *subject, const char *body);

#endif // GMAIL_MESSAGES_H
