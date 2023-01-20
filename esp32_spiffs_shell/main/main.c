#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <dirent.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_spiffs.h"

static const char *TAG = "SPIFFS";

#define LINE_MAX	100
char actual_path[256];

void ls_cmd(char* path) {

	printf("\r\nListing files in directory %s\r\n", path);
	
	DIR *dir;
	dir = opendir(path);
    if (!dir) {
        printf("Error opening directory\r\n");
        return;
    }
	
	// list the files and directories
	struct dirent *direntry;
	while ((direntry = readdir(dir)) != NULL) {
		
		if(strcmp(direntry->d_name, "/spiffs") == 0) continue;
		
		if(direntry->d_type == DT_DIR) printf("DIR\t");
		else if(direntry->d_type == DT_REG) printf("FILE\t");
		else printf("???\t");
		printf("%s\r\n", direntry->d_name);
	}
	
	// close the folder
	closedir(dir);
}

void cat_cmd(char* filename) {

	printf("\r\n[-] Content of the file %s\r\n", filename);
	
	// open the specified file
	char file_path[300];
	strcpy(file_path, actual_path);
	strcat(file_path, "/");
	strcat(file_path, filename);

	FILE *file;
	file = fopen(file_path, "r");
    if (!file) {
        printf("[-] Error opening file %s\r\n", file_path);
        return;
    }
	
	// get the file content
	int filechar;
	while((filechar = fgetc(file)) != EOF)
		putchar(filechar);
	
	// close the folder
	fclose(file);
}

void touch_cmd(char* touch) {
    printf("\n[+] Write and create new file:\n");
    // Create a file.
    ESP_LOGI(TAG, "Opening file");
    FILE *f = fopen("/spiffs/files/test.txt", "w");
    fprintf(f, "writing test\n");
    fclose(f);
}

void cd_cmd(char* path) {

	printf("\r\nMoving to directory %s \r\n", path);
	
	char previous_path[256];
	strcpy(previous_path, actual_path);
	
	if(strcmp(path, "..") == 0) {
		char* pos = strrchr(actual_path, '/');
		if(pos != actual_path) pos[0] = '\0';
	}

	else if(path[0] == '/') {
	
		strcpy(actual_path, "/spiffs");
		if(strlen(path) > 1) strcat(actual_path, path);
	}
	
	else {
		strcat(actual_path, "/");
		strcat(actual_path, path);
	}
	
	DIR *dir;
	dir = opendir(actual_path);
    
	if (!dir) {
        printf("Directory does not exists\r\n");
        strcpy(actual_path, previous_path);
		return;
    }
	
	closedir(dir);
}

void parse_cmd(char* command) {
	// split the command
	char* buffer;
	buffer = strtok(command, " ");
	
	if(!buffer) {
		printf("\r\nNo command provided!\r\n");
		return;
	}
	
	// LS command, list the content of the actual directory
	if(strcmp(buffer, "ls") == 0) {
	
		ls_cmd(actual_path);
	}
	
	// CAT command, display the content of a file
	else if(strcmp(buffer, "cat") == 0) {
	
		char* filename = strtok(NULL, " ");
		if(!filename) {
			
			printf("\r\nNo file specified!\r\n");
			return;
		}
		cat_cmd(filename);
	}
	
	// CD command, move to the specified directory
	else if(strcmp(buffer, "cd") == 0) {
	
		char* path = strtok(NULL, " ");
		if(!path) {
			
			printf("\r\n[-] No directory specified!\r\n");
			return;
		}
		cd_cmd(path);
	}
	
	else printf("\r\n[-] Unknown command!\r\n");
}

void print_prompt() {
	
	printf("\r\ndev01_esp32(");
	printf(actual_path);
	printf(")# ");
	fflush(stdout);
}

// main task
void main_task(void *pvParameter) {

	// buffer to store the command	
	char line_ctrl[LINE_MAX];
	int line_pos = 0;

	print_prompt();
	
	// read the command from stdin
	while(1) {
	
		int c = getchar();
		
		// when nothing to read, give to RTOS the control
		if(c < 0) {
			vTaskDelay(10 / portTICK_PERIOD_MS);
			continue;
		}
		if(c == '\r') continue;
		if(c == '\n') {
		
			line_ctrl[line_pos] = '\0';
			parse_cmd(line_ctrl);
			
			line_pos = 0;
			print_prompt();
		}
		else {
			putchar(c);
			line_ctrl[line_pos] = c;
			line_pos++;
			
			if(line_pos == LINE_MAX) {
				
				printf("\nCommand buffer full!\n");
				
				line_pos = 0;
				print_prompt();
			}
		}
	}	
}

void app_main(void)
{
    // SPIFFS init
    printf("\n[+] SPI Flash File System initiation: \n");
    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/spiffs",
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = true};
    esp_vfs_spiffs_register(&conf);
    size_t total = 0, used = 0;
    esp_spiffs_info(conf.partition_label, &total, &used);
    ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);

	strcpy(actual_path, "/spiffs");
	
	xTaskCreate(&main_task, "main_task", 8192, NULL, 5, NULL);

/* HOLD 
    // Unmount partition and disable SPIFFS
    printf("\nSPIFFS system unmount:\n");
    esp_vfs_spiffs_unregister(conf.partition_label);
    ESP_LOGI(TAG, "SPIFFS unmounted");

*/

}