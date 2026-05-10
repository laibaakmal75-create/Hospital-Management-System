#pragma once

static void copyString(char* dest, const char* source) {
	int k = 0;
	for (int i = 0; source[i] != 0; i++) {
		dest[k] = source[i];
		k++;
	}
	dest[k] = '\0';
}

static bool compareString(const char* str1, const char* str2) {
	int i = 0;
	while (str1[i] != '\0' && str2[i] != '\0') {
		if (str1[i] != str2[i]) {
			return false;
		}
		i++;
	}
	return str1[i] == str2[i];
}

//reads characters from line starting at pos into field until comma or end
//updates position to point after the comma
static void parseField(const char* line, int& pos, char* field) {
	int k = 0;
	while (line[pos] != ',' && line[pos] != '\0' && line[pos] != '\n') {
		field[k++] = line[pos++];
	}
	field[k] = '\0';
	if (line[pos] == ',') pos++; //skip comma
}

inline bool compareStringInsensitive(const char* a, const char* b) {
	int i = 0;
	while (a[i] != '\0' && b[i] != '\0') {
		char ca = a[i];
		char cb = b[i];
		// manual tolower without using any library function
		if (ca >= 'A' && ca <= 'Z') ca = ca + 32;
		if (cb >= 'A' && cb <= 'Z') cb = cb + 32;
		if (ca != cb) return false;
		i++;
	}
	return a[i] == '\0' && b[i] == '\0';
}