/*
* bitmap_font 
*
* A small header only library that uses allows you to load long bitmap fonts (PNG file with companion CSV file that 
* details font information).  This library depends on several other header only libraries so that SDL2_image binaries are
* not required to run.  
*
* Created by: Dave Guenther 2023
*
* Dependencies:
*     libSDL2         http://www.libsdl.org
*     stb_image.h     https://github.com/nothings/stb
*     SDL_stbimage.h  https://github.com/DanielGibson/Snippets/
*
* If you are already using stb_image.h in your source code, please ensure that this line is added somewhere (and only once) 
* in your project source code files.  Your root .cpp (main.cpp) is a good option
*
* #define SDL_STBIMAGE_IMPLEMENTATION
*
* If you are not using stb_image.h anywhere else in your source code and this header will be the only file it, then 
* uncomment the #define line above.
*
* Using this header requires that you establish a renderer context and pass that into the BitmapFont class.  It won't work with rendering
* approaches using SDL_UpdateWindowSurface(window);
*
*/


#ifndef BITMAP_FONT_H
#define BITMAP_FONT_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <memory>
#include <list>
#include <stdexcept>

#include <SDL2/SDL.h>

#include "SDL_stbimage.h" // 

class stbimageTexture{
    public:
        stbimageTexture(SDL_Renderer* renderer, const char* imageFilePath){
            this->renderer = renderer;
            this->loadTexture(imageFilePath);
        }
        ~stbimageTexture(){
            SDL_FreeSurface(this->image);
            SDL_DestroyTexture(this->textureImage);
        }

        void loadTexture(const char* imageFilePath){
            this->image = STBIMG_Load(imageFilePath);
            if(image == NULL) {
            printf("ERROR: Couldn't load %s, reason: %s\n", imageFilePath, SDL_GetError());
            exit(1);
            }
            this->textureImage = SDL_CreateTextureFromSurface(this->renderer, this->image);
        }

        SDL_Surface* getSurface(){
            return this->image;
        }

        SDL_Texture* getTexture(){
            return this->textureImage;
        }

        SDL_Renderer* getRenderer(){
            return this->renderer;
        }

    private:
        SDL_Renderer *renderer;
        SDL_Surface *image;
        SDL_Texture *textureImage;

};

class Glyph{
    public:
        Glyph(){}
        Glyph(std::shared_ptr<stbimageTexture> font_image_texture, SDL_Rect glyph_rect, int ascii_code, std::string ascii_char){
            loadGlyph(font_image_texture, glyph_rect, ascii_code, ascii_char);
        }
        ~Glyph(){
            //SDL_DestroyTexture(this->glyph_texture);

        }
        
        void loadGlyph(std::shared_ptr<stbimageTexture> font_image_texture, SDL_Rect glyph_rect, int ascii_code, std::string ascii_char){
            this->glyph_rect = glyph_rect;
            this->entire_font_texture = font_image_texture->getTexture();
            this->height = glyph_rect.h;
            this->width = glyph_rect.w;
            this->ascii_code = ascii_code;
            this->ascii_char = ascii_char;
                     
        }

        SDL_Texture* getFontTexture(){
            return this->entire_font_texture;
        }

        SDL_Rect* getGlyphOffsetRect(){
            return &(this->glyph_rect);
        }

        const std::string& getGlyphASCII_Char(){
            return this->ascii_char;
        }

        const int& getGlyphASCII_Code(){
            return this->ascii_code;
        }

        const int& getGlyphWidth(){
            return this->width;
        }

        const int& getGlyphHeight(){
            return this->height;
        }

    private:
        std::string ascii_char;
        int ascii_code;
        int width;
        int height;
        SDL_Rect glyph_rect;
        SDL_Texture* entire_font_texture;
};

typedef struct Row{
    int ASCII_code;
    std::string ASCII_chars;
    int cumulativeXPos;        
    int width;
}Row;

class CSV_Object{
    public:
        CSV_Object(std::string csv_font_map_fname){
            this->CSV_Load(csv_font_map_fname);

        }

        void CSV_Load(std::string csv_font_map_fname){

            std::string line, str_ascii_code, str_ascii_char, str_glyph_width;
            std::fstream csv_file (csv_font_map_fname, std::ios::in);       
            this->rows=0;
            this->ascii_codes.clear();
            std::cout << "Object Loaded" << std::endl;
            int ascii_code =0;
            int cumulative_xPos=0;
            int width=0;
            if(csv_file.is_open())
            {
                // Get first line of column names and discard
                if (getline(csv_file, line)){
                
                }
                
                // Get remaining lines
                while(getline(csv_file, line))
                {
                    //Each row should be formatted as follows:
                    //  {int ascii_code, std::string ascii_char, int glyph_width_in_pixels}
                    this->rows++;
                    std::stringstream ss(line);

                    if(getline(ss, str_ascii_code, ',')){
                        // first token is the ascii code of the character
                        ascii_code=std::stoi(str_ascii_code);
                        this->ascii_codes.push_back(ascii_code);
                    }else{
                        throw std::runtime_error(std::string("CSV Read Error: "+ csv_font_map_fname+ ": at row "+std::to_string(rows-1)+ ". Expected int for ASCII_code column."));
                    }

                    if(getline(ss,str_ascii_char,',')){
                        // second token is the ascii character (or description of character)
                    }else{
                        throw std::runtime_error(std::string("CSV Read Error: "+ csv_font_map_fname+": at row "+std::to_string(rows-1)+ ". Expected string for ASCII_char column."));
                    }

                    if(getline(ss,str_glyph_width,',')){
                        // This last token is a width in pixels of the glyph
                        width = std::stoi(str_glyph_width);

                    }else{
                        throw std::runtime_error(std::string("CSV Read Error: "+ csv_font_map_fname+ ": at row "+std::to_string(rows-1)+ ". Expected int for Width column."));
                    }
                    this->row_data.insert({ascii_code,{ascii_code,str_ascii_char,cumulative_xPos,width}});
                    cumulative_xPos+=width;                
                }
            }
            else
                throw std::runtime_error(std::string("Could not open file: " + csv_font_map_fname));
        }

        int getNumRows(){
            return this->rows;
        }
        
        /**
         * @brief Get the Row By I D object.  This provides a Zero indexed accessor to CSV row data.
         * 
         * @param id 
         * @return Row 
         */
        Row getRowByID(int id){
            if ((id>=0)&&(id<96)){            
                return this->row_data[this->ascii_codes[id]];
            }else{
                throw std::out_of_range(std::string("Row index out of range.  id must be [0-95]"));
            }                
        }

        /**
         * @brief Get the RowByASCII Code object.  This provides an ASCII_Code indexed accessor to CSV row data.
         * 
         * @param ascii_code 
         * @return Row 
         */
        Row getRowByASCII_Code(int ascii_code){
            if ((ascii_code>=32)&&(ascii_code<128)){
                return this->row_data[ascii_code];
            }else{
                throw std::out_of_range(std::string("ASCII Code out of range.  id must be [32-127]"));
            }
        }

    private:

        std::vector<int> ascii_codes;
        std::map<int, Row> row_data;
        int rows=0;
};


class GlyphRectScalar{
    public:
        GlyphRectScalar(){
                SDL_DisplayMode DM;
                SDL_GetCurrentDisplayMode(0, &DM);
                this->screen_width = float(DM.w);
                this->screen_height = float(DM.h);
        }

        /**
         * @brief Get the Adj Glyph Height.  Given a pt setting like in word (10pt, 12pt, etc) this function will calculate the height in pixels 
         * that a glyph typeset will have to display properly on a steamdeck.  No glyph is required to calculate this value sonce all glyphs will 
         * have the same height in a bitmap font.
         * 
         * @param pt 
         * @return int 
         */
        int getAdjGlyphHeight(int pt){
            // .352mm = physical height of 1pt font typeset
            // 93.98mm = physical height of screen in mm
            // 800px = vertical resolution of screen in pixels
            
            return std::floor(((.352*float(pt))/93.98)*800);//this->screen_height);
        }

        /**
         * @brief Get the Adj Glyph Width.  Given a Glyph and an adjusted rendering height in pixels for the glyph (all glyphs in a font have the same vertical typeset),
         * this function will determine the adjusted width in pixels
         * 
         * @param thisGlyph 
         * @param adjusted_glyph_height 
         * @return int
         */
        int getAdjGlyphWidth(std::shared_ptr<Glyph> thisGlyph, float adjusted_glyph_height){
            return std::floor((adjusted_glyph_height/float(thisGlyph->getGlyphHeight()))*float(thisGlyph->getGlyphWidth()));
        }

    private:
        float screen_width;
        float screen_height;
};


class WordWrap{
    public:
        /**
         * @brief Construct a new Word Wrap object
         * 
         * @param alphabetGlyphs this is a pointer to the entire alphabet of glyphs to be used by this class to render wordwrapped strings
         * @param input_string this is the std::string value that you wish to display on a workwrapped block
         * @param pt total fontsize (10pt, 12pt, 14pt, etc)
         * @param text_block_width Total width in pixels of the text block whereby the class with wrap the string to the next line
         */
        WordWrap(std::map<int,std::shared_ptr<Glyph>> alphabetGlyphs, std::string input_string, int pt, int text_block_width):
                                                                        alphabetGlyphs(alphabetGlyphs),
                                                                        input_string(input_string),
                                                                        pt(pt),
                                                                        text_block_width(text_block_width){
            
            tokenizeString();
            processTokens();
        }        
        


        std::vector<std::string> multi_line_strings;
        std::vector<int> multi_line_widths;
        int renderable_glyph_height_px;
    private:

        struct GlyphLine{
            std::list<std::shared_ptr<Glyph>> glyphs;
            std::list<SDL_Rect> renderable_rects;
            int width;
        };
        typedef struct GlyphLine GlyphLine;

        struct Token{
            std::string word;
            std::list<std::shared_ptr<Glyph>> word_glyphs;
            std::list<SDL_Rect> renderable_rects;
            int width;
        };
        typedef struct Token Token;

        std::vector<GlyphLine> renderable_lines;

        /**
         * @brief Given an existing GlyphLine and a token, this will break the token down into its glyph components and add each to the 
         * GlyphLine in place
         * 
         * @param this_line This is the Glyphline that will accept the token's glyphs (passed in by reference).  It is modified in place.
         * @param this_token This Token will be added to the Glyphline
         */
        void addTokenToLine(GlyphLine &this_line, Token &this_token){
            //this_token.renderable_rects.
            std::list<std::shared_ptr<Glyph>>::iterator glyph_iterator;
            glyph_iterator = this_token.word_glyphs.begin();
            std::list<SDL_Rect>::iterator rect_iterator;
            rect_iterator = this_token.renderable_rects.begin();
            while((glyph_iterator!=this_token.word_glyphs.end())&&
                    (rect_iterator!=this_token.renderable_rects.end())){ // move through each token individually

                this_line.glyphs.push_back(*glyph_iterator);
                this_line.renderable_rects.push_back(*rect_iterator);
                this_line.width+=rect_iterator->w;

                rect_iterator++;
                glyph_iterator++;
            }
        }

        /**
         * @brief carriageReturn() will accept a Token (likely the right-side remainder of a token that has likely been split) and set the x components of the renderable rects of that token such that they start at 0, rather than continuing on after the left end x value of the left-side token.
         * 
         * @param token_to_be_reset This token is passed by non-const reference to modify in place
         */
        void carriageReturn(Token &token_to_be_reset){
            std::list<SDL_Rect>::iterator renderable_rect_iterator;
            renderable_rect_iterator = token_to_be_reset.renderable_rects.begin();
            int last_glyph_width = renderable_rect_iterator->w;
            int current_glyph_x = 0;
            while(renderable_rect_iterator!=token_to_be_reset.renderable_rects.end()){ // move through each token individually
                renderable_rect_iterator->x=current_glyph_x;
                last_glyph_width=renderable_rect_iterator->w;
                renderable_rect_iterator++;
                current_glyph_x+=last_glyph_width;
            }
        }

        /**
         * @brief Given a Token, this will recalculate the renderable width in pixels (token.width) of that token on the screen based on the renderable rects it contains
         * 
         * @param token this is a token that has an accurate renerable_rects object, but may not have an updates token.width value
         */
        void calculateTokenRenderableWidth(Token &token){
            std::list<SDL_Rect>::iterator renderable_rect_iterator;
            renderable_rect_iterator = token.renderable_rects.begin();
            token.width=0;
            while(renderable_rect_iterator!=token.renderable_rects.end()){ // move through each token individually
                token.width+=renderable_rect_iterator->w;
                renderable_rect_iterator++;
            }            
        }

        /**
         * @brief This function trims the leading and trailing whitespace off std::string s and returns the string without those spaces.  The code for this function was taken from https://www.techiedelight.com/trim-string-cpp-remove-leading-trailing-spaces/
         * 
         * @param s This is an std::string that may have leading or trailing spaces in it which needs to be trimmed.
         * @return std::string without leading or trailing spaces
         */
        std::string trim(const std::string &s)
        {
            auto start = s.begin();
            while (start != s.end() && std::isspace(*start)) {
                start++;
            }
        
            auto end = s.end();
            do {
                end--;
            } while (std::distance(start, end) > 0 && std::isspace(*end));
        
            return std::string(start, end + 1);
        }     

        void processTokens(){
            GlyphLine this_line;
            std::string this_line_str;
            this_line.width=0;
            int space_ascii_code=32;
            int adj_glyph_height = this->glyph_scalar.getAdjGlyphHeight(pt);
            this->renderable_glyph_height_px=adj_glyph_height;
            int adj_glyph_width = this->glyph_scalar.getAdjGlyphWidth(this->alphabetGlyphs[space_ascii_code],adj_glyph_height);
            std::list<Token>::iterator token_iterator;
            token_iterator = tokenized_string.begin();
            int total_tokens = tokenized_string.size();
            int current_token = 0;
            while(current_token<total_tokens){ // move through each token individually

                if ((this_line.width==0) && (token_iterator->width > this->text_block_width-this_line.width)){
                    // A single token is longer than space allocated for a single line.  It will need to be chopped with a
                    // hyphen and the remaining amount handled on a new line as a new token

                    std::string left_word;
                    int char_width_px = token_iterator->renderable_rects.front().w;
                    int hyphen_width_px=char_width_px;
                    int space_width_px=char_width_px;
                    std::list<std::shared_ptr<Glyph>> left_word_glyphs;
                    std::list<SDL_Rect> left_renderable_rects;
                    int left_width=0;   
                    int char_index=0;
                    bool started_processing_token=false;                 
                    // split to left token and right token
                    for (int left_token_itr=0;left_token_itr<this->text_block_width-hyphen_width_px-this_line.width;left_token_itr+=char_width_px){
                        //Add front character of string to left_token
                        left_word+=token_iterator->word[char_index];
                        //Remove one character off front of string
                        token_iterator->word.erase(0,1);
                        // Add character to left token
                        left_word_glyphs.push_back(token_iterator->word_glyphs.front());

                        left_renderable_rects.push_back(token_iterator->renderable_rects.front());
                        left_width+=left_renderable_rects.front().w;
                        this_line.width+=left_renderable_rects.front().w;
                        token_iterator->width-=left_renderable_rects.front().w;
                        char_width_px=token_iterator->renderable_rects.front().w;
                        token_iterator->word_glyphs.pop_front();                     
                        token_iterator->renderable_rects.pop_front();
                    }   

                    // add hyphen to left token
                    int hyphen_ascii_code=45;
                    left_word+='-';
                    left_word_glyphs.push_back(this->alphabetGlyphs[hyphen_ascii_code]);
                    SDL_Rect scaled_hyphen_glyph_rect;
                    scaled_hyphen_glyph_rect.x=left_width;
                    scaled_hyphen_glyph_rect.y=0;
                    int adj_glyph_height = this->glyph_scalar.getAdjGlyphHeight(pt);
                    int adj_glyph_width = this->glyph_scalar.getAdjGlyphWidth(this->alphabetGlyphs[hyphen_ascii_code],adj_glyph_height);
                    scaled_hyphen_glyph_rect.h=adj_glyph_height;
                    scaled_hyphen_glyph_rect.w=adj_glyph_width;
                    left_renderable_rects.push_back(scaled_hyphen_glyph_rect);
                    Token left_token{left_word, left_word_glyphs, left_renderable_rects, left_width};
                    calculateTokenRenderableWidth(left_token);                    

                    // process left_token to a line and add the line
                    this->addTokenToLine(this_line,left_token);
                    renderable_lines.push_back(this_line);

                    //Now add left token string to multi_line_strings
                    this_line_str+=left_token.word;
                    this_line_str = trim(this_line_str);
                    this->multi_line_strings.push_back(this_line_str);
                    this->multi_line_widths.push_back(this_line.width);


                    // Clear the line now that it is stored on the vector
                    this_line.width=0;
                    this_line.glyphs.clear();
                    this_line.renderable_rects.clear();
                    this_line_str="";

                    // Note: We do NOT step the iterator because we trimmed the token to just the remainder,
                    // so we want to handle the remainder token over again in the next loop iteration, but need to reset the spacing so that it 
                    // begins at 0 like other tokens.
                    carriageReturn(*(token_iterator));
                    calculateTokenRenderableWidth(*(token_iterator));

                }
                else if(token_iterator->width > this->text_block_width-this_line.width){
                    

                    // Adding the current token to this line will push it over the total block width.  We'll not push the token and instead start a new line
                    renderable_lines.push_back(this_line);
                    this_line_str = trim(this_line_str);
                    this->multi_line_strings.push_back(this_line_str);
                    this->multi_line_widths.push_back(this_line.width);

                    // Clear the line now that it is stored on the vector
                    this_line.width=0;
                    this_line.glyphs.clear();
                    this_line.renderable_rects.clear();
                    this_line_str="";

                    // We do NOT step the iterator here because we want address this same token again on a new line

                }
                else if(token_iterator->width + token_iterator->renderable_rects.front().w > this->text_block_width-this_line.width){ 
                    // Adding the current token to this line will fit in the overall block width but adding a space on top 
                    // of that will push it over the total block width
                    this->addTokenToLine(this_line,*(token_iterator));
                    renderable_lines.push_back(this_line);

                    //Now add left token string to multi_line_strings
                    this_line_str+=token_iterator->word;
                    this_line_str = trim(this_line_str);
                    this->multi_line_strings.push_back(this_line_str);
                    this->multi_line_widths.push_back(this_line.width);

                    // Clear the line now that it is stored on the vector
                    this_line.width=0;
                    this_line.glyphs.clear();
                    this_line.renderable_rects.clear();
                    this_line_str="";

                    // We do step the iterator here because we want to look at the next token
                    current_token++;
                    if (current_token<total_tokens){ // safety so we don't step past the end of tokenized_string
                        token_iterator++;
                    }


                }
                else{
                    // Token and an added space will fit in the total block width.
                    Token token_w_space = *(token_iterator);
                    
                    // Add the space
                    int space_ascii_code=32;
                    token_w_space.word+=' ';
                    token_w_space.word_glyphs.push_back(this->alphabetGlyphs[space_ascii_code]);
                    SDL_Rect scaled_space_glyph_rect;
                    scaled_space_glyph_rect.x=token_w_space.renderable_rects.back().x+token_w_space.renderable_rects.back().w;
                    scaled_space_glyph_rect.y=0;
                    int adj_glyph_height = this->glyph_scalar.getAdjGlyphHeight(pt);
                    int adj_glyph_width = this->glyph_scalar.getAdjGlyphWidth(this->alphabetGlyphs[space_ascii_code],adj_glyph_height);
                    scaled_space_glyph_rect.h=adj_glyph_height;
                    scaled_space_glyph_rect.w=adj_glyph_width;
                    token_w_space.renderable_rects.push_back(scaled_space_glyph_rect);
                    calculateTokenRenderableWidth(token_w_space); //recalculate width bc we added a glyph
                    
                    this->addTokenToLine(this_line,token_w_space);

                    //Now add left token string to this_line_str
                    this_line_str+=token_w_space.word;
                  
                    
                    // If after adding the token to this_line, it's so big that it can't another glyph without exceeding the allowable text_block_width
                    // then the line is added to renderable_lines and a new line is created
                    if (this_line.width+adj_glyph_width>this->text_block_width){
                        renderable_lines.push_back(this_line);
                        this_line.glyphs.clear();
                        this_line.renderable_rects.clear();
                        this_line.width=0;
                        this_line_str="";
                    }

                    // We do step the iterator here because we want to look at the next token
                    current_token++;
                    if (current_token<total_tokens){ // safety so we don't step past the end of tokenized_string
                        token_iterator++;
                    }
                    
                }
            


            }
            // push the remaining line onto renderable_lines if it contains glyphs, and also add string content to multi_line_strings
            if(this_line.width>0){
                renderable_lines.push_back(this_line);
                this_line_str = trim(this_line_str);
                this->multi_line_strings.push_back(this_line_str);  
                this->multi_line_widths.push_back(this_line.width);
            }
        }


        void tokenizeString(){
            
            std::string this_word;
            std::list<std::shared_ptr<Glyph>> word_glyphs;
            std::list<SDL_Rect> renderable_rects;
            std::stringstream my_stream(this->input_string);
            while(getline(my_stream, this_word, ' ')){
                int cumulative_x=0;
                int adj_glyph_height;
                int adj_glyph_width;
                adj_glyph_height = this->glyph_scalar.getAdjGlyphHeight(pt);
                for (int ASCII_code:this_word){
                    if ((ASCII_code<32)||(ASCII_code>=128)){
                        ASCII_code=127;
                    }
                    adj_glyph_width = this->glyph_scalar.getAdjGlyphWidth(this->alphabetGlyphs[ASCII_code],adj_glyph_height);

                    SDL_Rect scaled_glyph_rect;
                    scaled_glyph_rect.x=cumulative_x;
                    scaled_glyph_rect.y=0;
                    scaled_glyph_rect.h=adj_glyph_height;
                    scaled_glyph_rect.w=adj_glyph_width;
                    cumulative_x = cumulative_x+adj_glyph_width;
                    renderable_rects.push_back(scaled_glyph_rect);
                    word_glyphs.push_back(this->alphabetGlyphs[ASCII_code]);
                }
                Token this_word_token{this_word, word_glyphs, renderable_rects, cumulative_x};
                this->tokenized_string.push_back(this_word_token);
                word_glyphs.clear();
                renderable_rects.clear();
            }
        }
 

        std::list<Token> tokenized_string;

        std::map<int,std::shared_ptr<Glyph>> alphabetGlyphs;
        GlyphRectScalar glyph_scalar;
        std::string input_string;
        int pt;
        int text_block_width;
};


class BitmapFont{
    public:
        BitmapFont(SDL_Renderer* renderer, std::string bitmap_fname, std::string csv_font_map_fname, int height_px):   
                                                                                renderer(renderer),
                                                                                bitmap_fname(bitmap_fname), 
                                                                                csv_font_map_fname(csv_font_map_fname),
                                                                                font_CSV(CSV_Object(csv_font_map_fname)),
                                                                                glyph_height_in_pixels(height_px){
            this->loadGlyphs();
            std::cout<<"CSV Loaded"<<std::endl;
        }


        /**
         * @brief This function will render a chracter glyph based on its ascii value at x, y pixel location on the screen.  The glyph will render at
         * the typeset font size provided by pt (10pt, 12pt, 14pt, etc) and is configured to display true to size on a Steamdeck Screen (as this library
         * was initially writted to work with SDL on the Steam Deck.)
         * 
         * @param x this is the x position to place the top left corner of the character
         * @param y this is the y position ot place the top left corner of the character
         * @param ascii_code this is the ascii code of the glyph to place (codes: 32-127 are supported)
         * @param pt the font size (10pt, 12pt, 14pt, etc)
         */
        void placeCharAtXY(int x, int y, int ascii_code, int pt){
            int scaled_height = glyph_scalar.getAdjGlyphHeight(pt);
            std::shared_ptr<Glyph> tempGlyph=this->alphabetGlyphs[ascii_code];
            int scaled_width = glyph_scalar.getAdjGlyphWidth(tempGlyph,scaled_height);
            SDL_Rect* glyphRect = tempGlyph->getGlyphOffsetRect();
            SDL_Rect scaled_glyph_rect;
            scaled_glyph_rect.x=x;
            scaled_glyph_rect.y=y;
            scaled_glyph_rect.h=scaled_height;
            scaled_glyph_rect.w=scaled_width;
            SDL_RenderCopy(renderer, tempGlyph->getFontTexture(),glyphRect,&scaled_glyph_rect);
        }

        /**
         * @brief This function will render this_string as a single line of glyphs at x, y pixel location on the screen.  The string will render at
         * the typeset font size provided by pt (10pt, 12pt, 14pt, etc) and is configured to display true to size on a Steamdeck Screen (as this library
         * was initially writted to work with SDL on the Steam Deck.)
         * 
         * @param this_strinng this is the string of characters to place
         * @param x this is the x position to place the top left corner of the first character of the string
         * @param y this is the y position to place the top left corner of the first character of the string
         * @param pt the font size (10pt, 12pt, 14pt, etc)
         */
        void placeStringAtXY(std::string this_string, int x, int y, int pt){
            int cumulative_x=x;
            int adj_glyph_height;
            int adj_glyph_width;
            adj_glyph_height = this->glyph_scalar.getAdjGlyphHeight(pt);
            for (int ASCII_code:this_string){
                if ((ASCII_code<32)||(ASCII_code>=128)){
                    ASCII_code=127;
                }
                adj_glyph_width = this->glyph_scalar.getAdjGlyphWidth(this->alphabetGlyphs[ASCII_code],adj_glyph_height);
                placeCharAtXY_Dimensions(cumulative_x, y, ASCII_code, adj_glyph_height, adj_glyph_width);
                cumulative_x = cumulative_x+adj_glyph_width;
            }

        }

        /**
         * @brief This function calculates the offset of the x value in pixels that a particular line of text needs to berendered to have it appear as left, right, or center justified.
         * 
         * @param line_width This is the width in pixels of a single line to be rendered after it has been scaled by the point value
         * @param max_width This is the maximum width of the text block that can hold wrapped text.
         * @param justification this is a std::string with possible values ["left","right","center"].  Default is "left"
         * @return int this function returns the offset in pixels to be applied to the x value when calling placeStringAtXY()
         */
        int justifyLineStartOffsetX(int line_width, int max_width, std::string justification){
            int offset_x = max_width-line_width;
            std::string right = "right";
            std::string center = "center";
            if(justification.compare(right)==0){
                // once we subtract the line_width from the max_width, we have the offset to add to each line to make it end at the right edge.
                // We don't have to do anything else here.  This branch is here for readability.
                offset_x=offset_x;
            }else if (justification.compare(center)==0){
                // if center justified, once we subtract the line_width from the max_width, we can simply divide that by 2 to get an offset that
                // will make the line of tect center-justified to the max_width
                offset_x = offset_x/2;
            }else{
                // Text is justified left by default.  This branch will just set the x_offset to zero so it remains left-justified.
                offset_x=0;
            }
            return offset_x;
        }

        /**
         * @brief This function will render this_string of Glyphs across multiple lines at x, y pixel location on the screen.  The string will 
         * render at the typeset font size provided by pt (10pt, 12pt, 14pt, etc) and is configured to display true to size on a Steamdeck Screen (as this library
         * was initially writted to work with SDL on the Steam Deck.)  The string is clipped and continues on the next line once the lkine_width is reached
         * 
         * @param this_strinng this is the string of characters to place
         * @param x this is the x position to place the top left corner of the first character of the string
         * @param y this is the y position to place the top left corner of the first character of the string
         * @param pt the font size (10pt, 12pt, 14pt, etc)
         * @param max_line_width this is the total width in pixels that the string needs to be placed in before moving to the next line
         * @param justification This is the justification to present for the wrapped text.  Possible options are ["left","right","center"].  Default is "left" justified
         */
        void placeWordWrappedStringAtXY(std::string this_string, int x, int y, int pt, int max_line_width, std::string justification){

            WordWrap my_word_wrapped_string(this->alphabetGlyphs, this_string, pt, max_line_width);
            int adj_line_height = my_word_wrapped_string.renderable_glyph_height_px;
            int line_number=0;
            int line_y=0;
            std::vector<int>::iterator line_width_iterator = my_word_wrapped_string.multi_line_widths.begin();
            std::vector<std::string>::iterator line_string_iterator = my_word_wrapped_string.multi_line_strings.begin();

            for (std::string this_line:my_word_wrapped_string.multi_line_strings){
                line_y=y+(adj_line_height*line_number);
                int x_offset = this->justifyLineStartOffsetX(my_word_wrapped_string.multi_line_widths[line_number], max_line_width, justification);
                placeStringAtXY(this_line,x+x_offset,line_y,pt);
                line_number+=1;
            }
            

        }

        std::shared_ptr<Glyph> getGlyph(int ASCII_code){
            if ((ASCII_code<32)||(ASCII_code>=128)){
                    ASCII_code=127;
                }
            return this->alphabetGlyphs[ASCII_code];
        }

    private:

        /**
         * @brief internal function called by the constructor to load all the glyphs in the ascii character set
         * 
         */
        void loadGlyphs(){

            this->font_image = std::shared_ptr<stbimageTexture>(new stbimageTexture(this->renderer, this->bitmap_fname.c_str()));
            //const char* my_c_str = this->bitmap_fname.c_str();
            for(int i=0;i<this->font_CSV.getNumRows();i++)
            {
                int ascii_code = font_CSV.getRowByID(i).ASCII_code;
                
                
                SDL_Rect glyphRect;
                glyphRect.x=font_CSV.getRowByASCII_Code(ascii_code).cumulativeXPos;
                glyphRect.y=0;
                glyphRect.h=this->glyph_height_in_pixels;
                glyphRect.w=font_CSV.getRowByASCII_Code(ascii_code).width;

                std::shared_ptr<Glyph> temp_Glyph(new Glyph(font_image, glyphRect, ascii_code,font_CSV.getRowByASCII_Code(ascii_code).ASCII_chars));                   
                std::cout << ascii_code << ", " << font_CSV.getRowByASCII_Code(ascii_code).ASCII_chars << ", " << font_CSV.getRowByASCII_Code(ascii_code).width << std::endl;
                alphabetGlyphs.insert({ascii_code,temp_Glyph});

            }            
        }

        /**
         * @brief This function will render a chracter glyph based on its ascii value at x, y pixel location on the screen.  Rather than 
         * using the pt fontsize to scale the glyph, it will instead capture a scaled_height and scaled_width variable to represent the bounding box
         * of the glyph in pixels on the screen.  This function is called by the placeString functions of this class.
         * 
         * @param x this is the x position to place the top left corner of the character
         * @param y this is the y position ot place the top left corner of the character
         * @param ascii_code this is the ascii code of the glyph to place (codes: 32-127 are supported)
         * @param scaled_height this is the height in pixels of the glyph's typeset bounding box
         * @param scaled_width this is the width in pixels of the glyph's typeset bounding box
         */
        void placeCharAtXY_Dimensions(int x, int y, int ascii_code, int scaled_height, int scaled_width){
            std::shared_ptr<Glyph> tempGlyph=this->alphabetGlyphs[ascii_code];
            SDL_Rect* glyphRect = tempGlyph->getGlyphOffsetRect();
            SDL_Rect scaled_glyph_rect;
            scaled_glyph_rect.x=x;
            scaled_glyph_rect.y=y;
            scaled_glyph_rect.h=scaled_height;
            scaled_glyph_rect.w=scaled_width;
            SDL_RenderCopy(renderer, tempGlyph->getFontTexture(),glyphRect,&scaled_glyph_rect);
        }

        std::map<int,std::shared_ptr<Glyph>> alphabetGlyphs;
        int glyph_height_in_pixels;
        SDL_Renderer* renderer;
        std::string bitmap_fname;
        std::string csv_font_map_fname;
        std::shared_ptr<stbimageTexture> font_image;
        GlyphRectScalar glyph_scalar;
        CSV_Object font_CSV;


};


#endif