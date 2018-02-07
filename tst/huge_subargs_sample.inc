struct optargs_argument * huge_subargs_sample = (struct optargs_argument [])
{
	{
		.type = optargs_argument_any,
		.name = "WHAEWA",
		.description = "Frisky first."
	},
	{
		.type = optargs_argument_group,
		.name = "CMD",
		.description = "Da command!"
	},
	{
		.type = optargs_argument_group_member,
		.name = "CMD0",
		.description = "Da command #0!",
	},
	{
		.type = optargs_argument_group_member,
		.name = "CMD1",
		.description = "Da command #1!",
		.subargument = (struct optargs_argument [])
		{
			{
				.type = optargs_argument_any,
				.name = "CMD1.ANY",
				.description = "Anything for CMD1!"
			},
			optargs_argument_eol
		}
	},
	{
		.type = optargs_argument_group_member,
		.name = "CMD2",
		.description = "Da command #2!",
		.subargument = (struct optargs_argument [])
		{
			{
				.type = optargs_argument_group,
				.name = "CMD2.GROUP",
				.description = "Group for CMD2."
			},
			{
				.type = optargs_argument_group_member,
				.name = "CMD2.GROUP.1",
				.description = "Group for CMD2 #1."
			},
			{
				.type = optargs_argument_group_member,
				.name = "CMD2.GROUP.2",
				.description = "Group for CMD2 #2."
			},
			{
				.type = optargs_argument_any,
				.name = "CMD2.ANY",
				.description = "Anything for CMD2 #2!"
			},
			{
				.type = optargs_argument_any,
				.name = "CMD2.ANY",
				.description = "Anything for CMD2 #3!"
			},
			optargs_argument_eol
		}
	},
	{
		.type = optargs_argument_group_member,
		.name = "CMD3",
		.description = "Da command #3!",
		.subargument = (struct optargs_argument [])
		{
			{
				.type = optargs_argument_group,
				.name = "CMD3.GROUP",
				.description = "Group for CMD3."
			},
			{
				.type = optargs_argument_group_member,
				.name = "CMD3.GROUP.1",
				.description = "Group for CMD3 #1.",
				.subargument = (struct optargs_argument [])
				{
					{
						.type = optargs_argument_any,
						.name = "CMD3.GROUP.1.ANY.1",
						.description = "Subargception #1."
					},
					{
						.type = optargs_argument_any_opt,
						.name = "CMD3.GROUP.1.ANY.2",
						.description = "Subargception #2."
					},
					optargs_argument_eol
				}
			},
			{
				.type = optargs_argument_group_member,
				.name = "CMD3.GROUP.2",
				.description = "Group for CMD3 #2.",
				.subargument = (struct optargs_argument [])
				{
					{
						.type = optargs_argument_group,
						.name = "CMD3.GROUP.2.GROUP",
						.description = "Subargception #1."
					},
					{
						.type = optargs_argument_group_member,
						.name = "CMD3.GROUP.2.GROUP.1",
						.description = "Subargception #1!!!",
						.subargument = (struct optargs_argument [])
						{
							{
								.type = optargs_argument_any,
								.name = "WTF",
								.description = "How can this even..."
							},
							optargs_argument_eol
						}
					},
					{
						.type = optargs_argument_group_member,
						.name = "CMD3.GROUP.2.GROUP.2",
						.description = "Subargception #2!!!"
					},
					optargs_argument_eol
				}
			},
			{
				.type = optargs_argument_group_member,
				.name = "CMD3.GROUP.3",
				.description = "Group for CMD3 #3."
			},
			optargs_argument_eol
		}
	},
	optargs_argument_eol
};

