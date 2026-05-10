#pragma once

template<typename T>
class Storage {
	T data[100];
	int count;
public:
	Storage() {
		count = 0;
	}

	//adding item to storage
	bool add(const T& item) {
		if (count < 100) {
			data[count] = item;
			count++;
			return true;
		}
		return false; //in case storage is full
	}

	//removing item by id
	bool removeById(int id) {
		for (int i = 0; i < count; i++) {
			if (data[i].getId() == id) {
				for(int j= i; j < count - 1; j++) {
					data[j] = data[j + 1];
				}
				count--;
				return true;
			}
		}
		return false;
	}

	//find by id 
	T* findById(int id) {
		for (int i = 0; i < count; i++) {
			if (data[i].getId() == id) {
				return &data[i];
			}
		}
		return nullptr;
	}

	T* getAll() {
		return data;
	}

	int size() const {
		return count;
	}

	void clear() {
		count = 0;
	}


};