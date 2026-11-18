#include "entity_textures.h"
#include "def.h"

unordered_map<string, i_sprite_texture> SPRITES;

int Init_Sprites(void) {
  json base_textures = io::read_json_file("data/entity_textures.json");
  if (base_textures == nullptr) {
    std::cerr<<"failed to read file/parse json: data/entity_textures.json\n";
    return 0;
  }

  for (auto i = base_textures.begin(); i != base_textures.end(); i++) {
    json& cur = i.value();

    unsigned nframes = (cur["num frames"].empty())   ? 0 : (int) cur["num frames"];
    uint64 uevery =    (cur["update every"].empty()) ? 0 : (int) cur["update every"];
    int oshot =        (cur["oneshot"].empty())      ? 0 : (int) cur["oneshot"];

    std::string tex_lookup;

    if (!cur["texture lookup name"].empty()) {
      tex_lookup = cur["texture lookup name"];
    } else {
      std::cerr<<"texture lookup name not provided in data/entity_textures.json\n";
      return 0;
    }

    if (SPRITES.count(tex_lookup)) {
      std::cerr<<"sprite already exists under the name "<<tex_lookup<<std::endl;
      return 0;
    }

    SPRITES[tex_lookup.c_str()] = {nullptr, tex_lookup, nframes, uevery, oshot};
    if (cur["textures"].empty()) {
      std::cerr<<"must define at least one texture for sprite "<<tex_lookup<<"\n";
      return 0;
    }

    size_t num_textures = cur["textures"].size();
    SPRITES[tex_lookup].tex = new Color[sq(OBJSIZE) * num_textures];

    for (size_t i = 0; i < num_textures; i++) {
      unsigned offset = i * sq(OBJSIZE);
      std::string tpath = cur["textures"][i];

      load_image_to_buff(tpath.c_str(), SPRITES[tex_lookup].tex + offset,
                         OBJSIZE, OBJSIZE);
    }
  }

  // here, read from a different file - the weapons file
  // weapons contain textures for when the player equips them

  /*
  // testing adding weapon textures to the sprites texture structure
  SPRITES["assets/sword.png"] = {0, "assets/sword.png", 4, 400, 1};
  SPRITES["assets/sword.png"].tex = new Color[sq(EQSIZE) * SPRITES["assets/sword.png"].num_frames];

  // remember the offsets for multiple sprite frames
  load_image_to_buff("assets/sword1.png", SPRITES["assets/sword.png"].tex,                EQSIZE, EQSIZE);
  load_image_to_buff("assets/sword2.png", SPRITES["assets/sword.png"].tex+(sq(EQSIZE)*1), EQSIZE, EQSIZE);
  load_image_to_buff("assets/sword3.png", SPRITES["assets/sword.png"].tex+(sq(EQSIZE)*2), EQSIZE, EQSIZE);
  load_image_to_buff("assets/sword3.png", SPRITES["assets/sword.png"].tex+(sq(EQSIZE)*3), EQSIZE, EQSIZE);
  */

  return 1;
}

int Init_Global_Sprites(void) {
  Init_Sprites();

  return 1;
}

void Free_Sprites() {
  for (const auto& pair : SPRITES) {
    delete pair.second.tex; // memory was only allocated for the texture, not the struct as a whole
  }
}
