#include "header/Moduls.h"
messageHistory::messageHistory(const messageHistory& x)
{
}
//messageHistory::messageHistory& operator=(const messageHistory& x)
//{
//}
messageHistory::messageHistory()
{
}
messageHistory::~messageHistory() {

}
messageHistory::messageHistory(messageHistory&& x) noexcept
{
	mess = std::move(x.mess);
	doc = std::move(x.doc);
	photo = std::move(x.photo);
	video = std::move(x.video);
}
td_api::file* messagemedia::GetFile_id(int file_id)
{
	td_api::file* file = NULL;
	if (mini)
	{
		if (this->mess->doc)
		{
			file = &(*mess->doc->document_->thumbnail_->file_);
		}
		else if (this->mess->photo)
		{
			file = &(*mess->photo->photo_->sizes_[0]->photo_);
		}
		else if (this->mess->video)
		{
			file = &(*mess->video->video_->thumbnail_->file_);
		}
	}
	else
	{

		if (this->mess->doc)
		{
			file = &(*mess->doc->document_->document_);
		}
		else if (this->mess->photo)
		{
			int len = mess->photo->photo_->sizes_.size();
			file = &(*mess->photo->photo_->sizes_[len - 1]->photo_);
		}
		else if (this->mess->video)
		{
			file = &(*mess->video->video_->video_);
		}
	}
	return file;
}
messagemedia::messagemedia()
{
}
messagemedia::~messagemedia() {

}
bool messagemedia::Que(messagemedia& mes)
{
	if (mes.GetFileID() == GetFileID())
	{
		return true;
	}
	else
	{
		return false;
	}
}
void messagemedia::SetMessageHistory(messageHistory* mess)
{
	this->mess = mess;
}
std::string messagemedia::GetPath()
{
	std::string path;
	if (mini)
	{
		if (mess->doc)
		{
			if (mess->doc->document_->thumbnail_)
				return mess->doc->document_->thumbnail_->file_->local_->path_;
		}
		else if (mess->photo)
		{
			return mess->photo->photo_->sizes_[0]->photo_->local_->path_;
		}
		else if (mess->video)
		{
			if (mess->video->video_->thumbnail_)
				return mess->video->video_->thumbnail_->file_->local_->path_;
		}
	}
	else
	{
		if (mess->doc)
		{
			return mess->doc->document_->document_->local_->path_;
		}
		else if (mess->photo)
		{
			int len = mess->photo->photo_->sizes_.size();
			return mess->photo->photo_->sizes_[len - 1]->photo_->local_->path_;
		}
		else if (mess->video)
		{
			return mess->video->video_->video_->local_->path_;
		}
	}
	return "";
}
void messagemedia::Usage()
{
	count_usage++;
}
int messagemedia::GetCountUsage()
{
	return count_usage;
}
void messagemedia::SetSize(int file_id, int size)
{
	td_api::file* file = GetFile_id(file_id);
	if (file)
	{
		file->local_->downloaded_size_ = size;
	}
}
void messagemedia::SetRemoteSize(int file_id, int size)
{
	td_api::file* file = GetFile_id(file_id);
	if (file)
	{
		file->remote_->uploaded_size_ = size;
	}
}
void messagemedia::SetPath(int file_id, std::string path) {
	td_api::file* file = GetFile_id(file_id);
	if (file)
	{
		file->local_->path_ = path;
	}
}
bool messagemedia::GetMini()
{
	return mini;
}
void messagemedia::SetMini(bool mini)
{
	this->mini = mini;
}
void TdMedia::Show()
{
	std::cout << "PATH: " << path << " REMOTE_ID: " << media.GetRemSize() << " SIZE: " << procLoad() << "%" << (int)mediaph << "\n";
}
int32_t messagemedia::GetSize()
{
	int32_t size = 0;
	if (mini)
	{
		if (mess->doc)
		{
			size = mess->doc->document_->thumbnail_->file_->local_->downloaded_size_;
		}
		else if (mess->video)
		{
			size = mess->video->video_->thumbnail_->file_->local_->downloaded_size_;
		}
		else if (mess->photo)
		{
			size = mess->photo->photo_->sizes_[0]->photo_->local_->downloaded_size_;
		}
	}
	else
	{
		if (mess->doc)
		{
			size = mess->doc->document_->document_->local_->downloaded_size_;
		}
		else if (mess->video)
		{
			size = mess->video->video_->video_->local_->downloaded_size_;
		}
		else if (mess->photo)
		{
			int len = mess->photo->photo_->sizes_.size();
			size = mess->photo->photo_->sizes_[len - 1]->photo_->local_->downloaded_size_;
		}
	}
	return size;
}
int messagemedia::GetFileID()
{
	int32_t size = 0;
	if (mini)
	{
		if (mess->doc)
		{
			if (mess->doc->document_->thumbnail_)
				size = mess->doc->document_->thumbnail_->file_->id_;
		}
		else if (mess->video)
		{
			if (mess->video->video_->thumbnail_)
				size = mess->video->video_->thumbnail_->file_->id_;
		}
		else if (mess->photo)
		{
			size = mess->photo->photo_->sizes_[0]->photo_->id_;
		}
	}
	else
	{

		if (mess->doc)
		{
			size = mess->doc->document_->document_->id_;
		}
		else if (mess->video)
		{
			size = mess->video->video_->video_->id_;
		}
		else if (mess->photo)
		{
			int len = mess->photo->photo_->sizes_.size();
			size = mess->photo->photo_->sizes_[len - 1]->photo_->id_;
		}
	}
	return size;
}
int32_t messagemedia::GetRemSize()
{
	int32_t size = 0;
	if (mini)
	{
		if (mess->doc)
		{
			size = mess->doc->document_->thumbnail_->file_->remote_->uploaded_size_;
		}
		else if (mess->video)
		{
			size = mess->video->video_->thumbnail_->file_->remote_->uploaded_size_;
		}
		else if (mess->photo)
		{
			size = mess->photo->photo_->sizes_[0]->photo_->remote_->uploaded_size_;
		}
	}
	else
	{
		if (mess->doc)
		{
			size = mess->doc->document_->document_->remote_->uploaded_size_;
		}
		else if (mess->video)
		{
			size = mess->video->video_->video_->remote_->uploaded_size_;
		}
		else if (mess->photo)
		{
			int len = mess->photo->photo_->sizes_.size();
			size = mess->photo->photo_->sizes_[len - 1]->photo_->remote_->uploaded_size_;
		}
	}
	return size;
}
void messagemedia::SetMessage(td_api::object_ptr<td_api::message> mess)
{
	this->mess->mess = std::move(mess);
}
void messagemedia::SetDocumentHistory(td_api::object_ptr<td_api::messageDocument> doc)
{
	this->mess->doc = std::move(doc);
}
void messagemedia::SetPhotoHistory(td_api::object_ptr<td_api::messagePhoto> photo)
{
	this->mess->photo = std::move(photo);
}
void messagemedia::SetVideoHistory(td_api::object_ptr<td_api::messageVideo> video)
{
	this->mess->video = std::move(video);
}
td_api::object_ptr<td_api::message>& messagemedia::GetMessage()
{
	return mess->mess;
}
int messagemedia::GetUsage()
{
	return count_usage;
}
bool3 messagemedia::GetMediaType() {
	if (mess->doc) {
		return 1;
	}
	else if (mess->photo) {
		return 0;
	}
	else if (mess->video) {
		return -1;
	}
}
std::string messagemedia::GetMediaTypeString() {
	if (mess->doc) {
		return "DOCUMENT";
	}
	else if (mess->photo) {
		return "PHOTO";
	}
	else if (mess->video) {
		return "VIDEO";
	}
}
float TdMedia::procLoad()
{
	float proc = 100 / ((float)media.GetSize() / (float)media.GetRemSize());
	return proc;
}
float TdMedia::procDown()
{
	float sizeloc = (float)media.GetSize();
	float sizeremot = (float)media.GetRemSize();
	float proc = 100 / (sizeremot / sizeloc);
	return proc;
}
TdMedia::TdMedia()
{
}
TdMedia::TdMedia(td_api::object_ptr<td_api::message> mess)
{
}
TdMedia::~TdMedia() {

}
bool3 TdMedia::GetMediaPh()
{
	return mediaph;
}
void TdMedia::SetMediaPh(bool3 media)
{
	this->mediaph = media;
}
std::string TdMedia::GetPath()
{
	return path;
}
void TdMedia::SetPath(std::string path)
{
	this->path = path;
}
Module::Module()
{
}
Module::Module(Tdlib* td)
{
	this->td = td;
}
Module::~Module() {

}
void Module::UpdatesModule(td_api::object_ptr<td_api::Object>& mod)
{
}
void Module::SetTd(Tdlib* td)
{
	this->td = td;
}
HistoryChat::HistoryChat()
{
	this->td = NULL;
}
HistoryChat::HistoryChat(Tdlib* td)
{
	this->td = td;
}
HistoryChat::~HistoryChat() {

}
bool HistoryChat::CheckMedia(td::tl_object_ptr<td::td_api::message>& mess)
{
	int32_t id = mess->content_->get_id();
	if (id == td_api::messageDocument::ID || id == td_api::messagePhoto::ID || id == td_api::messageVideo::ID)
		return true;
	else
		return false;
}
void HistoryChat::UpdateChatHistory()
{
	if (get_his == true)
	{
		messages.clear();
	}
	auto chath = td_api::make_object<td_api::getChatHistory>();
	chath->chat_id_ = td->GetChannelId();
	chath->from_message_id_ = last_mess;
	chath->limit_ = 100;
	td->send_query(std::move(chath), [this](td_api::object_ptr<td_api::Object> object)
		{
			if (object->get_id() == td_api::error::ID)
			{
				std::cout << "ERROR: " << object->get_id() << "\n";
				return;
			}
			auto mess = td::move_tl_object_as<td_api::messages>(object);
			if (mess->total_count_ != 0) {
				last_mess = mess->messages_[mess->total_count_ - 1]->id_;
				int size = 0;
				for (int i = 0; i < mess->total_count_; i++)
				{

					if (CheckMedia(mess->messages_[i])) {
						size++;
					}
				}
				int local = this->messages.size();
				this->messages.resize(local + size);
				int ide = local;
				for (int i = 0; i < mess->total_count_; i++)
				{

					if (CheckMedia(mess->messages_[i])) {

						int id = mess->messages_[i]->content_->get_id();

						if (id == td_api::messagePhoto::ID) {
							this->messages[ide].photo = td::move_tl_object_as<td_api::messagePhoto>(mess->messages_[i]->content_);
						}
						else if (id == td_api::messageDocument::ID) {
							this->messages[ide].doc = td::move_tl_object_as<td_api::messageDocument>(mess->messages_[i]->content_);
						}
						else if (id == td_api::messageVideo::ID) {
							this->messages[ide].video = td::move_tl_object_as<td_api::messageVideo>(mess->messages_[i]->content_);
						}

						this->messages[ide].mess = std::move(mess->messages_[i]);
						ide++;
					}
				}

				int last = messages.size();
				UpdateChatHistory();
				get_his = false;
			}
			else {

				last_mess = 0;
				std::cout << "MESSAGE COUN: " << messages.size() << "\n";
				get_his = true;
			} });
}
void HistoryChat::WaitHistory()
{
	int test = this->messages.size();
	while (get_his == false)
	{
	}
}

messagemedia HistoryChat::GetMessage(int index)
{
	messagemedia mess;
	mess.SetMessageHistory(&messages[index]);
	return mess;
}
void HistoryChat::AddMessageHistory(messageHistory mess)
{
	messages.push_back(std::move(mess));
}
int HistoryChat::GetCount()
{
	return messages.size();
}
void HistoryChat::Test()
{
	int in = messages.size();
}
TdFile::TdFile() {
	file = NULL;
}
TdFile::~TdFile() {

}
MenagerUDFile::MenagerUDFile()
{
	/* std::fstream fminiphoto;
	 fminiphoto.open("tdlib/thumbnails/miniphoto.txt", std::ios_base::in | std::ios_base::out);
	 if (fminiphoto.is_open())
	   procLogMiniPhoto(fminiphoto);
	 else
	   std::cout << "ERROR OPEN LOG\n";
	 fminiphoto.close();*/
}
MenagerUDFile::MenagerUDFile(Tdlib* td) : Module(td)
{
	/* std::fstream fminiphoto;
	 fminiphoto.open("tdlib/thumbnails/miniphoto.txt", std::ios_base::in | std::ios_base::out);
	 if (fminiphoto.is_open())
	   procLogMiniPhoto(fminiphoto);
	 else
	   std::cout << "ERROR OPEN LOG\n";
	 fminiphoto.close();*/
}
MenagerUDFile::~MenagerUDFile() {

}
void MenagerUDFile::procLogMiniPhoto(std::fstream& file)
{
	std::string str;

	while (std::getline(file, str))
	{

		for (int i = 0; i < str.size(); i++)
		{
			if (str[i] == ':')
			{
				TdFile fd;
				std::string temp;
				int size = str.size() - i - 1;
				temp.resize(size);
				fd.path.resize(i);
				memcpy(&fd.path[0], &str[0], i);
				memcpy(&temp[0], &str[i + 1], size);
				fd.count_usage = atoi(temp.c_str());
				this->miniphoto.push_back(fd);
				break;
			}
		}
	}
}
void MenagerUDFile::UpdatesModule(td_api::object_ptr<td_api::Object>& mod)
{
	td_api::downcast_call(
		*mod, overloaded(
			[this](td_api::updateFile& update_file)
			{
				int file = update_file.file_->local_->downloaded_size_;
				std::cout << "DOWN FILE: " << file << "\n";
			},
			[](auto& update) {}));
}
int MenagerUDFile::SearchVPhoto(std::string path, bool commpress, bool3 mediaph)
{
	for (int i = 0; i < loadphoto.size(); i++)
	{
		if (loadphoto[i].GetPath() == path && loadphoto[i].media.GetMini() == commpress && loadphoto[i].GetMediaPh() == mediaph)
		{
			return i;
		}
	}
	return -1;
}
int MenagerUDFile::SearchId(td_api::int64 id)
{
	for (int i = 0; i < loadphoto.size(); i++)
	{
		if (loadphoto[i].media.GetMessage()->id_ == id)
		{
			return i;
		}
	}
}
void MenagerUDFile::GetRometeSize(int id)
{
	auto remote_file = td_api::make_object<td_api::getMessage>();
	remote_file->chat_id_ = td->GetChannelId();
	remote_file->message_id_ = loadphoto[id].media.GetMessage()->id_;
	td->send_query(std::move(remote_file), [this](td_api::object_ptr<td_api::Object> object)
		{
			std::cout << "ROMOTE SIZE: " << "\n";
			if (object->get_id() == td_api::error::ID)
			{
				std::cout << "ERROR: " << object->get_id() << "\n";
				return;
			}
			auto mess = td::move_tl_object_as<td_api::message>(object);
			td_api::int32 size = 0;

			int id = SearchId(mess->id_);

			loadphoto[id].media.SetMessage(std::move(mess));
			loadphoto[id].Show();
			if (loadphoto[id].media.GetRemSize() != loadphoto[id].media.GetSize()) {
				GetRometeSize(id);
			}
			else {
				std::cout << "LOAD 100%\n";
			} });
}

void MenagerUDFile::LoadPhoto(std::string path, bool commpress)
{
	auto send_message = td_api::make_object<td_api::sendMessage>();
	send_message->chat_id_ = td->GetChannelId();
	TdMedia td;
	td.SetPath(path);
	td.media.SetMini(commpress);
	td.SetMediaPh(1);
	auto message_con = td_api::make_object<td_api::inputMessageDocument>();
	auto input_photo = td_api::make_object<td_api::inputFileLocal>();
	input_photo->path_ = path;
	message_con->document_ = std::move(input_photo);
	send_message->input_message_content_ = std::move(message_con);
	messageHistory mess;

	loadbuffer.push_back(mess);
	td.media.SetMessageHistory(&loadbuffer[loadbuffer.size() - 1]);
	this->td->send_query(std::move(send_message), [this](td_api::object_ptr<td_api::Object> object)
		{
			if (object->get_id() == td_api::error::ID)
			{
				std::cout << "ERROR: " << object->get_id() << "\n";
				return;
			}
			auto mess = td::move_tl_object_as<td_api::message>(object);
			int id_tdp = 0;
			td_api::int32 size = 0;
			auto ph = td::move_tl_object_as<td::td_api::messageDocument>(mess->content_);
			id_tdp = SearchVPhoto(ph->document_->document_->local_->path_, false, 1);
			this->loadphoto[id_tdp].media.SetDocumentHistory(std::move(ph));
			this->loadphoto[id_tdp].media.SetMessage(std::move(mess));
			GetRometeSize(id_tdp);
		});
	loadphoto.push_back(td);
}
void MenagerUDFile::ShowLoadPhoto()
{
	for (int i = 0; i < loadphoto.size(); i++)
	{
		loadphoto[i].Show();
	}
}
void MenagerUDFile::LoadVideo(std::string path)
{
	auto send_message = td_api::make_object<td_api::sendMessage>();
	send_message->chat_id_ = td->GetChannelId();
	TdMedia td;
	td.SetPath(path);
	// td.mediaph = 0;

	auto message_con = td_api::make_object<td_api::inputMessageVideo>();
	auto input_photo = td_api::make_object<td_api::inputFileLocal>();
	input_photo->path_ = path;
	message_con->width_ = 200;
	message_con->height_ = 100;
	message_con->video_ = std::move(input_photo);
	send_message->input_message_content_ = std::move(message_con);

	this->td->send_query(std::move(send_message), [this](td_api::object_ptr<td_api::Object> object)
		{
			if (object->get_id() == td_api::error::ID)
			{
				std::cout << "ERROR: " << object->get_id() << "\n";
				return;
			}
			auto mess = td::move_tl_object_as<td_api::message>(object);
			int id_tdp = 0;
			td_api::int32 size = 0;


			std::cout << "VIDEO SEND\n";
			auto vid = td::move_tl_object_as<td_api::messageVideo>(mess->content_);
			id_tdp = SearchVPhoto(vid->video_->video_->local_->path_, false, 0);
			messageHistory m;
			m.mess = std::move(mess);
			m.video = std::move(vid);
			//add history

			GetRometeSize(id_tdp); });
	loadphoto.push_back(td);
}

int MenagerUDFile::SearchLoc_id(int loc_id)
{
	for (int i = 0; i < downphoto.size(); i++)
	{
		if (downphoto[i]->media.GetFileID() == loc_id)
			return i;
	}
	return -1;
}

void MenagerUDFile::GetProcFile(int id)
{
	auto file = td_api::make_object<td_api::getFile>();
	int file_id = downphoto[id]->media.GetFileID();
	file->file_id_ = file_id;

	td->send_query(std::move(file), [this](td_api::object_ptr<td_api::Object> object)
		{

			if (object->get_id() == td_api::error::ID)
			{
				std::cout << "ERROR: " << object->get_id() << "\n";
				auto errr = td::move_tl_object_as<td_api::error>(object);
				std::cout << "ERROR: " << errr->message_ << "\n";
				return;
			}

			auto file = td::move_tl_object_as<td_api::file>(object);

			int index = SearchLoc_id(file->id_);
			downphoto[index]->media.SetPath(file->id_, file->local_->path_);
			downphoto[index]->media.SetSize(file->id_, file->local_->downloaded_size_);
			//if(downphoto[index]->procDown()!=0)
			//std::cout<<"DOWNLOAD: "<<downphoto[index]->procDown()<<"%"<<"\n";
			int locsize = downphoto[index]->media.GetSize();
			int remsize = downphoto[index]->media.GetRemSize();

			if (locsize < remsize) {
				GetProcFile(index);
			} });
}
void MenagerUDFile::SetHistoryChat(HistoryChat* hist)
{
	this->hist = hist;
}
TdMedia* MenagerUDFile::DownloadFile(messagemedia mess, bool mini)
{
	TdMedia* file;
	mess.SetMini(mini);
	if (mess.GetFileID() == 0)
		return NULL;
	std::string path = mess.GetPath();
	for (int i = 0; i < downphoto.size(); i++)
	{
		if (downphoto[i]->media.Que(mess))
		{
			file = downphoto[i];
			return file;
		}
	}


	TdMedia* med = new TdMedia();
	med->media = mess;

	downphoto.push_back(med);
	file = med;
	auto down = td_api::make_object<td_api::downloadFile>();
	down->file_id_ = mess.GetFileID();
	down->priority_ = 1;

	td->send_query(std::move(down), [this](td_api::object_ptr<td_api::Object> object)
		{
			//std::cout<<"OBJ\n";
			if (object->get_id() == td_api::error::ID)
			{
				std::cout << "ERROR: " << object->get_id() << "\n";
				auto errr = td::move_tl_object_as<td_api::error>(object);
				std::cout << "ERROR: " << errr->message_ << "\n";
				return;
			}

			auto file = td::move_tl_object_as<td_api::file>(object);
			int index = SearchLoc_id(file->id_);
			if (index == -1) {
				std::cout << "STOP";
				return;
			}
			downphoto[index]->media.SetPath(file->id_, file->local_->path_);
			downphoto[index]->media.SetSize(file->id_, file->local_->downloaded_size_);
			downphoto[index]->media.SetRemoteSize(file->id_, file->remote_->uploaded_size_);
			//std::cout<<"DOWNLOAD: "<<downphoto[index]->procDown()<<"%\n"; 
			GetProcFile(index); });

	return file;
}
