struct proctable {
  struct spinlock lock;
  struct proc proc[NPROC];
};
