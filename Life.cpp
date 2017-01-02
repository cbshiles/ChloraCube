#define ROUNDS 1000

void play(Cell cell){
  int i;
  for (i=0; i<ROUNDS; i++){
    cell.feed(1);
    cell.action();
    if cell.getEnergy() <= 0{
	delete cell;
      }
    
  }
}
