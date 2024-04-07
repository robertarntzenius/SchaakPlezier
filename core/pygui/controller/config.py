import toml
import os
import logging
from easydict import EasyDict

def load_config():
    config_path = os.path.join(os.path.dirname(__file__), 'config.toml')
    with open(config_path, 'r') as file:
        config = toml.load(file)
    
    log_level = logging._nameToLevel[config['defaults']['log_level']]
    logging.basicConfig(level=log_level, format='%(levelname)s - %(asctime)s - %(funcName)s - %(message)s')
    logging.info(f"log level set to: {config['defaults']['log_level']}")
    
    return EasyDict(config)

if __name__ == '__main__':
    config = load_config()