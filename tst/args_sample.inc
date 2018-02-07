struct optargs_argument args_sample[] =
{
	{ .name = "arg1", .description = "Argument #1" },
	{ .name = "arg2", .description = "Argument #2" },
	{ .name = "GROUP", .type = optargs_argument_group },
	{ .name = "g1", .type = optargs_argument_group_member, .description = "G1 I1" },
	{ .name = "g2", .type = optargs_argument_group_member, .description = "G1 I2"  },
	{ .name = "g3", .type = optargs_argument_group_member, .description = "G1 I3"  },
	optargs_argument_divider,
	{ .name = "arg3", .description = "Argument #4" },
	{ .name = "arg4", .description = "Argument #5", .type = optargs_argument_any_opt },
	{ .name = "GROUP2", .type = optargs_argument_group_opt },
	{ .name = "g4",.type = optargs_argument_group_member, .description = "G2 I1"  },
	{ .name = "g5",.type = optargs_argument_group_member, .description = "G2 I2"  },
	{ .name = "g6",.type = optargs_argument_group_member, .description = "G2 I3"  },
	optargs_argument_sink
};
