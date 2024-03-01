#pragma once

#include "pproto/commands/base.h"

namespace pproto {

typedef data::MessageError ErrorInfo;

namespace error {

//--- 10 Ошибки по работе с базой данных ---
DECL_ERROR_CODE(connect_to_database,          10, "2361b42b-7324-4230-8d00-a1c9348d7a58", u8"Ошибка подключения к базе данных")
DECL_ERROR_CODE(begin_transaction,            10, "d521450f-73f9-49a0-afe8-831dd66af353", u8"Ошибка старта транзакции")
DECL_ERROR_CODE(commit_transaction,           10, "5bc8301e-c1c5-4fa6-aeb8-c1b7b86afefe", u8"Ошибка завершения транзакции")
DECL_ERROR_CODE(rollback_transaction,         10, "ee3f3100-4643-4adf-8c6b-ca823e460d60", u8"Ошибка отмены транзакции")

DECL_ERROR_CODE(select_sql_statement,         10, "f99e821c-7f40-4c6e-aea2-b64ab5ec064c", u8"Ошибка выполнения 'select' sql-запроса")
DECL_ERROR_CODE(insert_sql_statement,         10, "141463a6-6cce-41c5-8a05-2473152ffe57", u8"Ошибка выполнения 'insert' sql-запроса")
DECL_ERROR_CODE(update_sql_statement,         10, "76861790-a004-4170-82e7-3ad608448645", u8"Ошибка выполнения 'update' sql-запроса")
DECL_ERROR_CODE(insert_or_update_sql,         10, "a1ba61bd-8c6d-464a-83e0-1db0d3f25246", u8"Ошибка выполнения 'insert or update' sql-запроса")
DECL_ERROR_CODE(delete_sql_statement,         10, "4ea9c633-0dec-4922-bdf7-3c5fa65f1075", u8"Ошибка выполнения 'delete' sql-запроса")

//--- 20 Ошибки общего плана ---
DECL_ERROR_CODE(usbrelay_not_attached,        20, "d1d8705a-89a9-4dce-a332-3019f47a6391", u8"Usb-реле не подключено")
DECL_ERROR_CODE(usbrelay_turn_on,             20, "667b388a-3243-4106-868e-5e0bb9e2365b", u8"Не удалось включить usb-реле")
DECL_ERROR_CODE(usbrelay_already_turn_on,     20, "f8a34df1-40f8-4ace-a49b-92893252585a", u8"Usb-реле уже включено")
DECL_ERROR_CODE(admin_password,               20, "3a32b445-3930-4408-94ed-2280c797ecb9", u8"Неверный пароль администратора")
//DECL_ERROR_CODE(cannot_write_file_journal,  20, "9f56517e-5dac-4cbc-a09f-2cf3491ea0a6", u8"Невозможно сохранить файл журнала: %1")


//--- 30 Ошибки по работе с видео-событиями ---
DECL_ERROR_CODE(videoevent_not_found,         30, "2ac2a3b7-0568-4268-bab0-cdcd65d6d325", u8"Видеособытие с идентификатором  %1 не найдено")
DECL_ERROR_CODE(videoevent_data_read,         30, "57f07bc8-2b04-49a2-9751-9bd4d9cb820e", u8"Ошибка чтения данных из потока.  Идентификатор видеособытия %1")
DECL_ERROR_CODE(videoevent_chunk_timeout,     30, "5409d01a-3607-47e7-8f72-6deffc291bc7", u8"Превышено время ожидания ответа от клиента.  Идентификатор видеособытия %1")
DECL_ERROR_CODE(videosaver_is_active,         30, "258cd3a4-2217-4386-a8af-02752cc93321", u8"Механизм записи видео уже активен")
//DECL_ERROR_CODE(request_is_null,            30, "c936a2f9-510c-4229-aae2-ecd5440c5300", "Request ID is null")

////--- 40 Ошибки по работе с алгоритмами ---
//DECL_ERROR_CODE(algo_is_null,                 40, "9f603b40-cb7d-48c2-b370-5a2406ad863b", "Algorithm ID is null")
//DECL_ERROR_CODE(algo_is_exist,                40, "70cf1761-df8e-4265-abab-51cefd192699", "Algorithm ID not unique")

//--- 50 Ошибки по работе с json ---
//DECL_ERROR_CODE(json_parse_error,             50, "fd665c40-c2e1-4697-b673-b15e6db9c846", "Json parse error")

//--- 60 Ошибки Application
DECL_ERROR_CODE(init_appl_id,                 60, "fa12fdda-d57b-4d62-ab3a-3d08af75c529", u8"Ошибка чтения  ID  Application в Config file")
DECL_ERROR_CODE(init_appl_name,               60, "56bdcfed-db72-477e-82d7-0cde29f5672c", u8"Ошибка чтения Name Application в Config file")
DECL_ERROR_CODE(init_appl_saverPath,          60, "61bdeaeb-652a-4965-b94c-65155bd1e597", u8"Ошибка чтения SaverPath Application в Config file")


} // namespace error
} // namespace pproto
