/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lottavi <lottavi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 21:09:20 by lottavi           #+#    #+#             */
/*   Updated: 2024/12/22 01:18:48 by lottavi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

#define MAX_LINE_LENGTH 256

// Funzione per verificare se una stringa rappresenta un numero valido
int is_valid_number(const char *str) {
    if (!str || *str == '\0') return 0;
    for (int i = 0; str[i]; i++) {
        if (!isdigit(str[i])) return 0;
    }
    return 1;
}

// Funzione per verificare il formato del colore
int is_valid_color(const char *str) {
    char *token;
    char buffer[MAX_LINE_LENGTH];
    strncpy(buffer, str, MAX_LINE_LENGTH);

    int count = 0;
    token = strtok(buffer, ",");
    while (token) {
        if (!is_valid_number(token)) return 0;
        int value = atoi(token);
        if (value < 0 || value > 255) return 0;
        count++;
        token = strtok(NULL, ",");
    }
    return count == 3;
}

// Funzione per verificare che una linea non contenga caratteri extra
int has_extra_characters(const char *line) {
    for (int i = 0; line[i]; i++) {
        if (!isdigit(line[i]) && line[i] != ',' && line[i] != ' ' && line[i] != '\n' && line[i] != '\0') {
            return 1;
        }
    }
    return 0;
}

int validate_floor_color(const char *line, int *found_f) {
    if (has_extra_characters(line) || !is_valid_color(line)) {
        printf("Errore: colore del pavimento (F) non valido o contiene caratteri extra.\n");
        return 0;
    }
    (*found_f)++;
    return 1;
}

int validate_ceiling_color(const char *line, int *found_c) {
    if (has_extra_characters(line) || !is_valid_color(line)) {
        printf("Errore: colore del soffitto (C) non valido o contiene caratteri extra.\n");
        return 0;
    }
    (*found_c)++;
    return 1;
}

int validate_map_line(const char *line) {
    if (line[0] != '1' && line[0] != '0' && line[0] != ' ' && line[0] != 'N' && line[0] != 'S' && line[0] != 'W' && line[0] != 'E') {
        printf("Errore: linea non riconosciuta o fuori ordine: '%s'\n", line);
        return 0;
    }
    return 1;
}

int check_directives_count(int found_no, int found_so, int found_we, int found_ea, int found_f, int found_c) {
    printf("NO: %d\nSO: %d\nWE: %d\nEA: %d\nF: %d\nC: %d\n", found_no, found_so, found_we, found_ea, found_f, found_c);
    if (found_no != 1 || found_so != 1 || found_we != 1 || found_ea != 1 || found_f != 1 || found_c != 1) {
        printf("Errore: mancano alcune direttive obbligatorie o sono duplicate.\n");
        return 0;
    }
    return 1;
}

int validate_line(const char *line, int *found_no, int *found_so, int *found_we, int *found_ea, int *found_f, int *found_c) {
    if (strncmp(line, "NO ", 3) == 0) {
        (*found_no)++;
    } else if (strncmp(line, "SO ", 3) == 0) {
        (*found_so)++;
    } else if (strncmp(line, "WE ", 3) == 0) {
        (*found_we)++;
    } else if (strncmp(line, "EA ", 3) == 0) {
        (*found_ea)++;
    } else if (strncmp(line, "F ", 2) == 0) {
        if (!validate_floor_color(line + 2, found_f)) return 0;
    } else if (strncmp(line, "C ", 2) == 0) {
        if (!validate_ceiling_color(line + 2, found_c)) return 0;
    } else if (*line == '\0' || *line == '\n') {
        return 1;
    } else {
        if (!validate_map_line(line)) return 0;
    }
    return 1;
}


int validate_cub_file(const char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("Errore nell'apertura del file");
        return 0;
    }

    char *line;
    int found_no = 0, found_so = 0, found_we = 0, found_ea = 0, found_f = 0, found_c = 0;

    while ((line = get_next_line(fd)) != NULL) {
        line[strcspn(line, "\n")] = '\0';
        if (!validate_line(line, &found_no, &found_so, &found_we, &found_ea, &found_f, &found_c)) {
            free(line);
            close(fd);
            return 0;
        }
        free(line);
    }

    close(fd);
    return check_directives_count(found_no, found_so, found_we, found_ea, found_f, found_c);
}
